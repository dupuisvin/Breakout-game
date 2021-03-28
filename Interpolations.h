#ifndef M_INTERPOLATIONS
#define M_INTERPOLATIONS

namespace SDLEngine
{
    //Base on the Math for Game Programmers: Fast and Funky 1D Nonlinear Transformation GDC talk by Squirrel Eiserloh 
    template<class T> static T Interpolate(T v0, T v1, float t) { return (1.0f - t) * v0 + t * v1; }
    template <class T> static T SmoothStart2(T val) { return val * val; }
    template <class T> static T SmoothStart3(T val) { return val * val * val; }
    template <class T> static T SmoothStart4(T val) { return val * val * val * val; }
    template <class T> static T SmoothStop2(T val) { return 1 - SmoothStart2<T>(1 - val); }
    template <class T> static T SmoothStop3(T val) { return 1 - SmoothStart3<T>(1 - val); }
    template <class T> static T SmoothStop4(T val) { return 1 - SmoothStart4<T>(1 - val); }
    template <class T, class EaseFuncA, class EaseFuncB>
    static T MixEaseFunc(EaseFuncA funcA, EaseFuncB funcB, float blend, float t)
    {
        return (1 - blend) * funcA(t) + blend * funcB(t);
    }
    // SmoothStart2.2 = MixEaseFunc(SmoothStart2, SmoothStart3, 0.2);


    //Cubic Bezier through A,B,C,D where A(start) and D (end) are assumed to be 0 and 1
    static float NormalziedBezier3(float B, float C, float t)
    {
        float s = 1.f - t;
        float t2 = t * t;
        float s2 = s * s;
        float t3 = t2 * t;
        return (3.f * B * s2 * t) + (3.f * C * s * t2) + t3;
    }

    template<class T, class EaseFunc> static T RangeMap(T in, T inStart, T inEnd, T outStart, T outEnd, EaseFunc func)
    {
        T out = in - inStart; //[0, inEnd-inStart]
        out /= (inEnd - inStart); //[0,1]
        out = func(out); //Easing/Tweening function on [0,1]
        out *= (outEnd - outStart); //[0, outRange]
        return out + outStart; //[outStart, outEnd]
    }
}

#endif // !M_INTERPOLATIONS
