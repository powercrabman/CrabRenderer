#pragma once

namespace crab
{

// ==================================================
// Draw2D
// --------------------------------------------------
// this class is for simple drawing 2D shapes
// like line, circle, rectangle, etc.
// you can draw sprite, text, etc.
//
// this class use instancing for drawing multiple shapes
// so you can draw multiple shapes with one draw call
// this is efficient.
//
// this is not immediate rendering
// batch rendering is used
//
// this class use own shader and vertex
// if you want to use custom shader
// now this is not supported
// ==================================================

class Draw2D
{
public:
    static void Init();

private:
    struct Impl;
    static Scope<Impl> m_impl;
};

}   // namespace crab