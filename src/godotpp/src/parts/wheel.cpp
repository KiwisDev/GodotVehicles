#include "parts/wheel.h"

using namespace godot;

GVWheel::GVWheel() {}

GVWheel::~GVWheel() {}

void GVWheel::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_radius", "radius"), &GVWheel::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &GVWheel::get_radius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");

    ClassDB::bind_method(D_METHOD("set_suspension_rest_length", "length"), &GVWheel::set_suspension_rest_length);
    ClassDB::bind_method(D_METHOD("get_suspension_rest_length"), &GVWheel::get_suspension_rest_length);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "suspension_rest_length"), "set_suspension_rest_length", "get_suspension_rest_length");

    ClassDB::bind_method(D_METHOD("set_suspension_stiffness", "stiffness"), &GVWheel::set_suspension_stiffness);
    ClassDB::bind_method(D_METHOD("get_suspension_stiffness"), &GVWheel::get_suspension_stiffness);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "suspension_stiffness"), "set_suspension_stiffness", "get_suspension_stiffness");

    ClassDB::bind_method(D_METHOD("set_suspension_damping", "damping"), &GVWheel::set_suspension_damping);
    ClassDB::bind_method(D_METHOD("get_suspension_damping"), &GVWheel::get_suspension_damping);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "suspension_damping"), "set_suspension_damping", "get_suspension_damping");

    ClassDB::bind_method(D_METHOD("set_is_steering", "is_steering"), &GVWheel::set_is_steering);
    ClassDB::bind_method(D_METHOD("get_is_steering"), &GVWheel::get_is_steering);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_steering"), "set_is_steering", "get_is_steering");

    ClassDB::bind_method(D_METHOD("set_is_drive", "is_drive"), &GVWheel::set_is_drive);
    ClassDB::bind_method(D_METHOD("get_is_drive"), &GVWheel::get_is_drive);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_drive"), "set_is_drive", "get_is_drive");
}