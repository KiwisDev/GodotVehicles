#ifndef GODOTVEHICLES_VEHICLES_H
#define GODOTVEHICLES_VEHICLES_H

#include <vector>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>

#include "godot_cpp/classes/immediate_mesh.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "parts/wheel.h"

namespace godot
{
    struct DebugLine {
        Vector3 start;
        Vector3 end;
        Color color;
    };

    class GVVehicle : public RigidBody3D
    {
        GDCLASS(GVVehicle, RigidBody3D);

    private:
        std::vector<GVWheel*> wheels;

        float engine_force = 0.0f;
        float steering_angle = 0.0f;
        float max_grip = 10.0f;
        bool show_debug_lines = false;

        std::vector<DebugLine> debug_lines;
        MeshInstance3D* debug_mesh_instance;
        Ref<ImmediateMesh> debug_mesh;

        void find_wheels(Node* current_node);

    protected:
        static void _bind_methods();

    public:
        GVVehicle();
        ~GVVehicle();

        void _ready() override;

        void _process(double p_delta) override;

        void _physics_callback(PhysicsDirectBodyState3D* state);

        void set_engine_force(float p_force) { engine_force = p_force; }
        float get_engine_force() const { return engine_force; }

        void set_steering_angle(float p_angle) { steering_angle = p_angle; }
        float get_steering_angle() const { return steering_angle; }

        void set_max_grip(float p_grip) { max_grip = p_grip; }
        float get_max_grip() const { return max_grip; }

        void set_show_debug_lines(bool p_debug) { show_debug_lines = p_debug; }
        bool get_show_debug_lines() const { return show_debug_lines; }
    };
}

#endif //GODOTVEHICLES_VEHICLES_H
