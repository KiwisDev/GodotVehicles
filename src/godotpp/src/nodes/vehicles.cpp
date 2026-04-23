#include "nodes/vehicles.h"

#include "godot_cpp/classes/physics_direct_body_state3d.hpp"
#include "godot_cpp/classes/physics_direct_space_state3d.hpp"
#include "godot_cpp/classes/physics_ray_query_parameters3d.hpp"
#include "godot_cpp/classes/standard_material3d.hpp"
#include "godot_cpp/classes/world3d.hpp"

using namespace godot;

GVVehicle::GVVehicle() {}

GVVehicle::~GVVehicle() {}

void GVVehicle::_ready()
{
    debug_mesh_instance = memnew(MeshInstance3D);
    debug_mesh.instantiate();
    debug_mesh_instance->set_mesh(debug_mesh);
    debug_mesh_instance->set_as_top_level(true);

    Ref<StandardMaterial3D> mat;
    mat.instantiate();
    mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
    mat->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
    mat->set_flag(BaseMaterial3D::FLAG_DISABLE_DEPTH_TEST, true);
    debug_mesh_instance->set_material_override(mat);

    add_child(debug_mesh_instance);

    set_can_sleep(false);
    wheels.clear();
    find_wheels(this);

    PhysicsServer3D::get_singleton()->body_set_force_integration_callback(
        get_rid(),
        callable_mp(this, &GVVehicle::_physics_callback)
        );
}

void GVVehicle::_process(double p_delta)
{
    if (show_debug_lines == true)
    {
        debug_mesh->clear_surfaces();

        if (!debug_lines.empty())
        {
            debug_mesh->surface_begin(Mesh::PRIMITIVE_LINES);
            for (const DebugLine& line : debug_lines)
            {
                debug_mesh->surface_set_color(line.color);
                debug_mesh->surface_add_vertex(line.start);
                debug_mesh->surface_set_color(line.color);
                debug_mesh->surface_add_vertex(line.end);
            }
            debug_mesh->surface_end();
        }
    }
}

void GVVehicle::find_wheels(Node* current_node)
{
    GVWheel* wheel = Object::cast_to<GVWheel>(current_node);
    if (wheel != nullptr)
    {
        wheels.push_back(wheel);
    }

    for (int i=0; i < current_node->get_child_count(); ++i)
    {
        find_wheels(current_node->get_child(i));
    }
}

void GVVehicle::_physics_callback(PhysicsDirectBodyState3D* state)
{
    debug_lines.clear();

    if (wheels.empty()) return;

    PhysicsDirectSpaceState3D* space_state = get_world_3d()->get_direct_space_state();
    float delta = state->get_step();

    for (GVWheel* wheel : wheels)
    {
        Vector3 car_up = get_global_basis().get_column(1).normalized();
        Vector3 wheel_down = -car_up;
        Vector3 wheel_forward = -get_global_basis().get_column(2).normalized();
        Vector3 wheel_right = get_global_basis().get_column(0).normalized();

        if (wheel->get_is_steering())
        {
            wheel_forward = wheel_forward.rotated(car_up, steering_angle);
            wheel_right = wheel_right.rotated(car_up, steering_angle);

            Vector3 visual_rot = wheel->get_rotation();
            visual_rot.y = wheel->get_initial_rotation() + steering_angle;
            wheel->set_rotation(visual_rot);
        }

        Vector3 wheel_pos = wheel->get_global_position();
        float ray_length = wheel->get_suspension_rest_length() + wheel->get_radius();

        Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(wheel_pos, wheel_pos + (wheel_down * ray_length));
        query->set_exclude(TypedArray<RID>::make(get_rid()));

        Dictionary hit = space_state->intersect_ray(query);

        if (!hit.is_empty())
        {
            // Suspension
            Vector3 hit_pos = hit["position"];
            float distance = wheel_pos.distance_to(hit_pos);

            float spring_length = distance - wheel->get_radius();
            float compression = wheel->get_suspension_rest_length() - spring_length;

            Vector3 local_pos = wheel_pos - get_global_position();
            Vector3 point_vel = state->get_velocity_at_local_position(local_pos);

            float compression_vel = (compression - wheel->get_previous_compression()) / delta;
            wheel->set_previous_compression(compression);

            float spring_force = (compression * wheel->get_suspension_stiffness()) + (compression_vel * wheel->get_suspension_damping());
            spring_force = Math::max(0.0f, spring_force);

            Vector3 hit_normal = hit["normal"];
            state->apply_force(hit_normal * spring_force, local_pos);

            // DEBUG //
            debug_lines.push_back({hit_pos, hit_pos + (car_up * (spring_force / 1000.0f)), Color(1, 0, 0)});

            // Longitudinal
            if (wheel->get_is_drive())
            {
                state->apply_force(wheel_forward * engine_force, local_pos);
                // DEBUG //
                debug_lines.push_back({hit_pos, hit_pos + (wheel_forward * (engine_force / 1000.0f)), Color(0, 1, 0)});
            }

            // Lateral
            float lateral_vel = point_vel.dot(wheel_right);

            float desired_accel = -lateral_vel / delta;
            float mass_supported = get_mass() / wheels.size();
            float friction_force = desired_accel * mass_supported;

            friction_force = CLAMP(friction_force, -max_grip * 100.0f, max_grip * 100.0f);
            state->apply_force(wheel_right * friction_force, local_pos);

            // DEBUG //
            debug_lines.push_back({hit_pos, hit_pos + (wheel_right * (friction_force / 1000.0f)), Color(0, 0, 1)});
        }
        else
        {
            wheel->set_previous_compression(0.0f);
        }
    }
}

void GVVehicle::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_physics_callback", "state"), &GVVehicle::_physics_callback);

    ClassDB::bind_method(D_METHOD("set_engine_force", "force"), &GVVehicle::set_engine_force);
    ClassDB::bind_method(D_METHOD("get_engine_force"), &GVVehicle::get_engine_force);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "engine_force"), "set_engine_force", "get_engine_force");

    ClassDB::bind_method(D_METHOD("set_steering_angle", "angle"), &GVVehicle::set_steering_angle);
    ClassDB::bind_method(D_METHOD("get_steering_angle"), &GVVehicle::get_steering_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "steering_angle"), "set_steering_angle", "get_steering_angle");

    ClassDB::bind_method(D_METHOD("set_max_grip", "max_grip"), &GVVehicle::set_max_grip);
    ClassDB::bind_method(D_METHOD("get_max_grip"), &GVVehicle::get_max_grip);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_grip"), "set_max_grip", "get_max_grip");

    ClassDB::bind_method(D_METHOD("set_show_debug_lines", "show_debug_lines"), &GVVehicle::set_show_debug_lines);
    ClassDB::bind_method(D_METHOD("get_show_debug_lines"), &GVVehicle::get_show_debug_lines);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_debug_lines"), "set_show_debug_lines", "get_show_debug_lines");
}