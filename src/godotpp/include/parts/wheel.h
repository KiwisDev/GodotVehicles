#ifndef GODOTVEHICLES_WHEELS_H
#define GODOTVEHICLES_WHEELS_H

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
    class GVWheel : public Node3D
    {
        GDCLASS(GVWheel, Node3D);

    private:
        float radius = 0.35f;
        float suspension_rest_length = 0.5f;
        float suspension_stiffness = 50.0f;
        float suspension_damping = 4.0f;

        bool is_steering = false;
        bool is_drive = false;

        float previous_compression = 0.0f;
        float initial_rotation = 0.0f;

    protected:
        static void _bind_methods();

    public:
        GVWheel();
        ~GVWheel();

        void _ready() override;

        void set_radius(float p_radius) { radius = p_radius; }
        float get_radius() const { return radius; }

        void set_suspension_rest_length(float p_length) { suspension_rest_length = p_length; }
        float get_suspension_rest_length() const { return suspension_rest_length; }

        void set_suspension_stiffness(float p_stiffness) { suspension_stiffness = p_stiffness; }
        float get_suspension_stiffness() const { return suspension_stiffness; }

        void set_suspension_damping(float p_damping) { suspension_damping = p_damping; }
        float get_suspension_damping() const { return suspension_damping; }

        void set_is_steering(bool p_steering) { is_steering = p_steering; }
        bool get_is_steering() const { return is_steering; }

        void set_is_drive(bool p_drive) { is_drive = p_drive; }
        bool get_is_drive() const { return is_drive; }

        void set_previous_compression(float p_comp) { previous_compression = p_comp; }
        float get_previous_compression() const { return previous_compression; }

        float get_initial_rotation() const { return initial_rotation; }
    };
}

#endif //GODOTVEHICLES_WHEELS_H
