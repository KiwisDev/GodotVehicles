#ifndef GODOTVEHICLES_VEHICLES_H
#define GODOTVEHICLES_VEHICLES_H

#include <vector>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>

#include "parts/wheel.h"

namespace godot
{
    class GVVehicle : public RigidBody3D
    {
        GDCLASS(GVVehicle, RigidBody3D);

    private:
        std::vector<GVWheel*> wheels;

        float engine_force = 0.0f;
        float steering_angle = 0.0f;
        float max_grip = 10.0f;

        void find_wheels(Node* current_node);

    protected:
        static void _bind_methods();

    public:
        GVVehicle();
        ~GVVehicle();

        void _ready() override;

        void _integrate_forces(PhysicsDirectBodyState3D* state) override;

        void set_engine_force(float p_force) { engine_force = p_force; }
        float get_engine_force() const { return engine_force; }

        void set_steering_angle(float p_angle) { steering_angle = p_angle; }
        float get_steering_angle() const { return steering_angle; }
    };
}

#endif //GODOTVEHICLES_VEHICLES_H
