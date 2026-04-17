#include "nodes/vehicles.h"

#include "godot_cpp/classes/physics_direct_body_state3d.hpp"
#include "godot_cpp/classes/physics_direct_space_state3d.hpp"
#include "godot_cpp/classes/physics_ray_query_parameters3d.hpp"
#include "godot_cpp/classes/world3d.hpp"

using namespace godot;

GVVehicle::GVVehicle() {}

GVVehicle::~GVVehicle() {}

void GVVehicle::_ready()
{
    set_use_custom_integrator(true);
    set_can_sleep(false);
    wheels.clear();
    find_wheels(this);
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

void GVVehicle::_integrate_forces(PhysicsDirectBodyState3D* state)
{
    state->apply_central_force(Vector3(0.0f, 1000.0f, 0.0f));
    if (wheels.empty()) return;

    PhysicsDirectSpaceState3D* space_state = get_world_3d()->get_direct_space_state();
    float delta = state->get_step();

    for (GVWheel* wheel : wheels)
    {
        if (wheel->get_is_steering())
        {
            Vector3 rotation = wheel->get_rotation();
            rotation.y = steering_angle;
            wheel->set_rotation(rotation);
        }

        Vector3 wheel_pos = wheel->get_global_position();
        Vector3 wheel_down = -wheel->get_global_basis().get_column(1).normalized();
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

            Vector3 local_pos = wheel_pos - wheel->get_global_position();
            Vector3 point_vel = state->get_velocity_at_local_position(local_pos);

            float compression_vel = (compression - wheel->get_previous_compression()) / delta;
            wheel->set_previous_compression(compression);

            float spring_force = (compression * wheel->get_suspension_stiffness()) + (compression_vel * wheel->get_suspension_damping());
            spring_force = Math::max(0.0f, spring_force);

            Vector3 hit_normal = hit["normal"];
            state->apply_force(hit_normal * spring_force, local_pos);

            // Longitudinal
            if (wheel->get_is_drive())
            {
                Vector3 forward_dir = -wheel->get_global_basis().get_column(2).normalized();
                state->apply_force(forward_dir * engine_force, local_pos);
            }

            // Lateral
            Vector3 right_dir = wheel->get_global_basis().get_column(0).normalized();
            float lateral_vel = point_vel.dot(right_dir);

            float desired_accel = -lateral_vel / delta;
            float mass_supported = get_mass() / wheels.size();
            float friction_force = desired_accel * mass_supported;

            friction_force = CLAMP(friction_force, -max_grip * 100.0f, max_grip * 100.0f);
            state->apply_force(right_dir * friction_force, local_pos);
        }
        else
        {
            wheel->set_previous_compression(0.0f);
        }
    }
}

void GVVehicle::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_engine_force", "force"), &GVVehicle::set_engine_force);
    ClassDB::bind_method(D_METHOD("get_engine_force"), &GVVehicle::get_engine_force);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "engine_force"), "set_engine_force", "get_engine_force");

    ClassDB::bind_method(D_METHOD("set_steering_angle", "angle"), &GVVehicle::set_steering_angle);
    ClassDB::bind_method(D_METHOD("get_steering_angle"), &GVVehicle::get_steering_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "steering_angle"), "set_steering_angle", "get_steering_angle");
}
