#include <doctest_with_main.h>

import core.math;

using namespace draco;

TEST_SUITE("math") {
    TEST_CASE("pow") {
        f32 result = math::pow(2.0f, 0.5f);
        constexpr f32 expected = math::SQRT2;
        CHECK_EQ(result, expected);
    }

    TEST_CASE("abs") {
        using math::abs;

        RAC_CHECK_EQ(abs(-1.f), 1.f);
        RAC_CHECK_EQ(abs(4.56f), 4.56f);
        RAC_CHECK_EQ(abs(-1.), 1.);
        RAC_CHECK_EQ(abs(4.56), 4.56);
        RAC_CHECK_EQ(abs(-5), 5);
        RAC_CHECK_EQ(abs(3L), 3L);
        RAC_CHECK_EQ(abs(-32L), 32L);
        RAC_CHECK_EQ(abs(5000ULL), 5000ULL);
    }
}

TEST_SUITE("vector2") {
    TEST_CASE("constructors") {
        using math::Vector2;
        using math::Vector3;
        using math::Vector4;

        static constexpr Vector3 a{1.0f, 2.0f, 3.0f};
        static constexpr Vector4 b{4.0f, 5.0f, 6.0f, 7.0f};
        
        BASIC_RAC_SUBCASE("float",
            ( Vector2(1.0f) ),
            ( Vector2{1.0f, 1.0f} )
        );
        
        BASIC_RAC_SUBCASE("vec3",
            ( Vector2(a) ),
            ( Vector2{1.0f, 2.0f} )
        );
        
        BASIC_RAC_SUBCASE("vec4",
            ( Vector2(b) ),
            ( Vector2{4.0f, 5.0f} )
        );
    }

    TEST_CASE("access") {
        using math::Vector2;

        static constexpr Vector2 v(1.0f, 2.0f);

        RAC_CHECK_EQ(v[0], 1.0f);
        RAC_CHECK_EQ(v[1], 2.0f);
    }

    TEST_CASE("swizzle") {
        using math::Vector2;
        using math::Vector3;
        using math::Vector4;
        
        static constexpr Vector2 v{1.0f, 2.0f};

        BASIC_RAC_SUBCASE("vec2",
            ( v[1, 0] ),
            ( Vector2{2.0f, 1.0f} )
        );

        BASIC_RAC_SUBCASE("vec3",
            ( v[1, 1, 0] ),
            ( Vector3{2.0f, 2.0f, 1.0f} )
        );

        BASIC_RAC_SUBCASE("vec4",
            ( v[0, 1, 1, 0] ),
            ( Vector4{1.0f, 2.0f, 2.0f, 1.0f} )
        );
    }

    TEST_CASE("swap") {
        using math::Vector2;

        Vector2 a{1.f, 2.f};
        Vector2 b{2.f, 1.f};

        std::swap(a, b);

        CHECK_EQ(a, Vector2{2.f, 1.f});
        CHECK_EQ(b, Vector2{1.f, 2.f});
    }

    TEST_CASE("dot") {
        using math::Vector2;
        using math::dot;

        static constexpr Vector2 a{1.0f, 2.0f};
        static constexpr Vector2 b{3.0f, 4.0f};

        BASIC_RAC_SUBCASE("basic",
            ( dot(a, b) ),
            ( 11.0f )
        );

        BASIC_RAC_SUBCASE("self",
            ( dot(a, a) ),
            ( 5.0f )
        );

        BASIC_RAC_SUBCASE("zero",
            ( dot(a, Vector2()) ),
            ( 0.0f )
        );
    }

    TEST_CASE("length") {
        using math::Vector2;
        using math::length;
        using math::length_sq;

        static constexpr Vector2 v{3.0f, 4.0f};

        BASIC_R_SUBCASE("normal",
            ( length(v) ),
            ( 5.0f )
        );

        BASIC_RAC_SUBCASE("squared",
            ( length_sq(v) ),
            ( 25.0f )
        );
    }

    TEST_CASE("distance") {
        using math::Vector2;
        using math::distance;
        using math::distance_sq;

        static constexpr Vector2 a{3.0f, 4.0f};
        static constexpr Vector2 b{-3.0f, 12.0f};

        BASIC_R_SUBCASE("normal",
            ( distance(a, b) ),
            ( 10.0f )
        );

        BASIC_RAC_SUBCASE("squared",
            ( distance_sq(a, b) ),
            ( 100.0f )
        );
    }

    TEST_CASE("normalize") {
        using math::Vector2;
        using math::length;
        using math::normalize;
        using math::normalize_fast;

        static constexpr Vector2 a{3.0f, 4.0f};
        static constexpr Vector2 b(1e-99);

        const Vector2 result = normalize(a);
        const Vector2 result_fast = normalize_fast(a);
        const Vector2 result_zero = normalize(b);

        CHECK_EQ(length(result), 1.0f);
        CHECK_EQ(result, result_fast);
        CHECK_EQ(result_zero, Vector2());
    }

    TEST_CASE("project") {
        using math::Vector2;
        using math::project;

        static constexpr Vector2 a{4.0f, 6.0f};
        static constexpr Vector2 b{2.0f, 2.0f};

        RAC_CHECK_EQ(
            ( project(a, b) ),
            ( Vector2{5.0f, 5.0f} )
        );
    }

    TEST_CASE("reflect") {
        using math::Vector2;
        using math::reflect;

        static constexpr Vector2 a{1.0f, 2.0f};
        static constexpr Vector2 b{3.0f, 4.0f};

        RAC_CHECK_EQ(
            ( reflect(a, b) ),
            ( Vector2{-65.0f, -86.0f} )
        );
    }

    TEST_CASE("angle") {
        using math::Vector2;
        using math::angle;
        using math::PI2;

        static constexpr Vector2 a{2.0f, 1.0f};
        static constexpr Vector2 b{-2.0f, 4.0f};

        R_CHECK_EQ(
            ( angle(a, b) ),
            ( PI2 )
        );
    }

    TEST_CASE("lerp") {
        using math::Vector2;
        using math::lerp;

        static constexpr Vector2 a{1.0f, 2.0f};
        static constexpr Vector2 b{3.0f, 4.0f};

        BASIC_RAC_SUBCASE("weight = -1",
            ( lerp(a, b, -1.0f) ),
            ( Vector2{-1.0f, 0.0f} )
        );

        BASIC_RAC_SUBCASE("weight = 0",
            ( lerp(a, b, 0.0f) ),
            ( a )
        );

        BASIC_RAC_SUBCASE("weight = 0.5",
            ( lerp(a, b, 0.5f) ),
            ( Vector2{2.0f, 3.0f} )
        );

        BASIC_RAC_SUBCASE("weight = 1",
            ( lerp(a, b, 1.0f) ),
            ( b )
        );

        BASIC_RAC_SUBCASE("weight = 2",
            ( lerp(a, b, 2.0f) ),
            ( Vector2{5.0f, 6.0f} )
        );
    }

    TEST_CASE("min") {
        using math::Vector2;
        using math::min;

        static constexpr Vector2 a{5.0f, 3.0f};
        static constexpr Vector2 b{1.0f, 7.0f};

        BASIC_RAC_SUBCASE("vector",
            ( min(a, b) ),
            ( Vector2{1.0f, 3.0f} )
        );

        static constexpr Vector2 expected{4.0f, 3.0f};

        BASIC_RAC_SUBCASE_2("float",
            ( min(a, 4.0f) ),
            ( expected ),
            ( min(4.0f, a) ),
            ( expected )
        );
    }

    TEST_CASE("min_length") {
        using math::Vector2;
        using math::length;
        using math::min_length;

        static constexpr Vector2 a{3.0f, 4.0f};  // len: 5
        static constexpr Vector2 b{5.0f, 12.0f}; // len: 13

        BASIC_RAC_SUBCASE("vector",
            ( min_length(a, b) ),
            ( a )
        );

        SUBCASE("float") {
            static constexpr f32 smaller_length = 1.0f;
            static constexpr f32 larger_length = 10.0f;

            const Vector2 result_smaller = min_length(a, smaller_length);
            const Vector2 result_swapped = min_length(smaller_length, a);
            const Vector2 result_larger = min_length(larger_length, a);

            CHECK_EQ(length(result_smaller), smaller_length);
            CHECK_EQ(result_smaller, result_swapped);
            CHECK_EQ(result_larger, a);
        }
    }

    TEST_CASE("max") {
        using math::Vector2;
        using math::max;

        static constexpr Vector2 a{5.0f, 3.0f};
        static constexpr Vector2 b{1.0f, 7.0f};

        BASIC_RAC_SUBCASE("vector",
            ( max(a, b) ),
            ( Vector2{5.0f, 7.0f} )
        );

        static constexpr Vector2 expected{5.0f, 4.0f};

        BASIC_RAC_SUBCASE_2("float",
            ( max(a, 4.0f) ),
            ( expected ),
            ( max(4.0f, a) ),
            ( expected )
        );
    }

    TEST_CASE("max_length") {
        using math::Vector2;
        using math::length;
        using math::max_length;

        static constexpr Vector2 a{3.0f, 4.0f};  // len: 5
        static constexpr Vector2 b{5.0f, 12.0f}; // len: 13

        BASIC_RAC_SUBCASE("vector",
            ( max_length(a, b) ),
            ( b )
        );

        SUBCASE("float") {
            static constexpr f32 smaller_length = 1.0f;
            static constexpr f32 larger_length = 10.0f;

            const Vector2 result_smaller = max_length(a, smaller_length);
            const Vector2 result_swapped = max_length(larger_length, a);
            const Vector2 result_larger = max_length(larger_length, a);

            CHECK_EQ(length(result_larger), larger_length);
            CHECK_EQ(result_larger, result_swapped);
            CHECK_EQ(result_smaller, a);
        }
    }

    TEST_CASE("clamp") {
        using math::Vector2;
        using math::clamp;

        static constexpr Vector2 a{5.0f, 3.0f};
        static constexpr Vector2 b{1.0f, 7.0f};
        static constexpr Vector2 c{3.0f, 8.0f};

        BASIC_RAC_SUBCASE("vector",
            ( clamp(a, b, c) ),
            ( Vector2{3.0f, 7.0f} )
        );

        BASIC_RAC_SUBCASE("float",
            ( clamp(a, 4.0f, 5.0f) ),
            ( Vector2{5.0f, 4.0f} )
        );
    }

    TEST_CASE("clamp_length") {
        using math::Vector2;
        using math::length;
        using math::clamp_length;

        static constexpr Vector2 v{3.0f, 4.0f}; // len: 5

        BASIC_R_SUBCASE("length < min",
            ( clamp_length(v, 10.0f, 15.0f) ),
            ( Vector2{6.0f, 8.0f} )
        );

        BASIC_R_SUBCASE("length == min",
            ( clamp_length(v, 5.0f, 10.0f) ),
            ( v )
        );

        BASIC_R_SUBCASE("length == max",
            ( clamp_length(v, 3.0f, 5.0f) ),
            ( v )
        );

        BASIC_R_SUBCASE("length > max",
            ( clamp_length(v, 1.0f, 2.5f) ),
            ( Vector2{1.5f, 2.0f} )
        );
    }

    TEST_CASE("abs") {
        using math::Vector2;
        using math::abs;

        RAC_CHECK_EQ(
            ( abs(Vector2{1.0f, -2.0f}) ),
            ( Vector2{1.0f, 2.0f} )
        );
    }

    TEST_CASE("rounding") {
        using math::Vector2;
        using math::floor;
        using math::ceil;
        using math::trunc;
        using math::round;
        
        static constexpr Vector2 a{0.5f, 1.4f};
        static constexpr Vector2 b{-1.0f, 1.0f};

        BASIC_RAC_SUBCASE_2("floor",
            ( floor(a) ),
            ( Vector2{0.0f, 1.0f} ),
            ( floor(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("ceil",
            ( ceil(a) ),
            ( Vector2{1.0f, 2.0f} ),
            ( ceil(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("trunc",
            ( trunc(a) ),
            ( Vector2{0.0f, 1.0f} ),
            ( trunc(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("round",
            ( round(a) ),
            ( Vector2{1.0f, 1.0f} ),
            ( round(b) ),
            ( b )
        );
    }

    TEST_CASE("sign") {
        using math::Vector2;
        using math::sign;

        RAC_CHECK_EQ(
            ( sign(Vector2{1.0f, -1.0f}) ),
            ( Vector2{1.0f, -1.0f} )
        );
    }

    TEST_CASE("approx_eq") {
        using math::Vector2;
        using math::approx_eq;
        using math::CMP_EPSILON;

        static constexpr Vector2 v{1.0f, 2.0f};
        static constexpr Vector2 offset = Vector2::xAxis(CMP_EPSILON);

        BASIC_R_SUBCASE("distance < epsilon",
            ( approx_eq(v, v + offset * 0.5f) ),
            ( true )
        );

        BASIC_R_SUBCASE("distance == epsilon",
            ( approx_eq(v, v + offset) ),
            ( true )
        );

        BASIC_R_SUBCASE("distance > epsilon",
            ( approx_eq(v, v + offset * 2.0f) ),
            ( false )
        );
    }
}

TEST_SUITE("vector3") {
    TEST_CASE("constructors") {
        using math::Vector2;
        using math::Vector3;
        using math::Vector4;

        static constexpr Vector2 a{1.0f, 2.0f};
        static constexpr Vector4 b{3.0f, 4.0f, 5.0f, 6.0f};
        
        BASIC_RAC_SUBCASE("float",
            ( Vector3(1.0f) ),
            ( Vector3{1.0f, 1.0f, 1.0f} )
        );
        
        BASIC_RAC_SUBCASE("vec2, float",
            ( Vector3(a, 7.0f) ),
            ( Vector3{1.0f, 2.0f, 7.0f} )
        );
        
        BASIC_RAC_SUBCASE("float, vec2",
            ( Vector3(7.0f, a) ),
            ( Vector3{7.0f, 1.0f, 2.0f} )
        );
        
        BASIC_RAC_SUBCASE("vec4",
            ( Vector3(b) ),
            ( Vector3{3.0f, 4.0f, 5.0f} )
        );
    }

    TEST_CASE("access") {
        using math::Vector3;

        static constexpr Vector3 v(1.0f, 2.0f, 3.0f);

        RAC_CHECK_EQ(v[0], 1.0f);
        RAC_CHECK_EQ(v[1], 2.0f);
        RAC_CHECK_EQ(v[2], 3.0f);
    }

    TEST_CASE("swizzle") {
        using math::Vector2;
        using math::Vector3;
        using math::Vector4;
        
        static constexpr Vector3 v{1.0f, 2.0f, 3.0f};

        BASIC_RAC_SUBCASE("vec2",
            ( v[1, 0] ),
            ( Vector2{2.0f, 1.0f} )
        );

        BASIC_RAC_SUBCASE("vec3",
            ( v[1, 2, 0] ),
            ( Vector3{2.0f, 3.0f, 1.0f} )
        );

        BASIC_RAC_SUBCASE("vec4",
            ( v[0, 2, 1, 0] ),
            ( Vector4{1.0f, 3.0f, 2.0f, 1.0f} )
        );
    }

    TEST_CASE("swap") {
        using math::Vector3;

        Vector3 a{1.f, 2.f, 3.f};
        Vector3 b{3.f, 2.f, 1.f};

        std::swap(a, b);

        CHECK_EQ(a, Vector3{3.f, 2.f, 1.f});
        CHECK_EQ(b, Vector3{1.f, 2.f, 3.f});
    }

    TEST_CASE("dot") {
        using math::Vector3;
        using math::dot;

        static constexpr Vector3 a{1.0f, 2.0f, 3.0f};
        static constexpr Vector3 b{4.0f, 5.0f, 6.0f};

        BASIC_RAC_SUBCASE("basic",
            ( dot(a, b) ),
            ( 32.0f )
        );

        BASIC_RAC_SUBCASE("self",
            ( dot(a, a) ),
            ( 14.0f )
        );

        BASIC_RAC_SUBCASE("zero",
            ( dot(a, Vector3()) ),
            ( 0.0f )
        );
    }

    TEST_CASE("length") {
        using math::Vector3;
        using math::length;
        using math::length_sq;

        static constexpr Vector3 v{2.0f, 4.0f, 4.0f};

        BASIC_R_SUBCASE("normal",
            ( length(v) ),
            ( 6.0f )
        );

        BASIC_RAC_SUBCASE("squared",
            ( length_sq(v) ),
            ( 36.0f )
        );
    }

    TEST_CASE("distance") {
        using math::Vector3;
        using math::distance;
        using math::distance_sq;

        static constexpr Vector3 a{2.0f, 4.0f, 4.0f};
        static constexpr Vector3 b{-1.0f, -2.0f, -2.0f};

        BASIC_R_SUBCASE("normal",
            ( distance(a, b) ),
            ( 9.0f )
        );

        BASIC_RAC_SUBCASE("squared",
            ( distance_sq(a, b) ),
            ( 81.0f )
        );
    }

    TEST_CASE("normalize") {
        using math::Vector3;
        using math::length;
        using math::normalize;
        using math::normalize_fast;

        static constexpr Vector3 a{0.0f, 6.4f, 4.8f};
        static constexpr Vector3 b(1e-99);

        const Vector3 result = normalize(a);
        const Vector3 result_fast = normalize_fast(a);
        const Vector3 result_zero = normalize(b);

        CHECK_EQ(length(result), 1.0f);
        CHECK_EQ(result, result_fast);
        CHECK_EQ(result_zero, Vector3());
    }

    TEST_CASE("project") {
        using math::Vector3;
        using math::project;

        static constexpr Vector3 a{2.0f, 8.0f, 4.0f};
        static constexpr Vector3 b{1.0f, 1.0f, 2.0f};

        RAC_CHECK_EQ(
            ( project(a, b) ),
            ( Vector3{3.0f, 3.0f, 6.0f} )
        );
    }

    TEST_CASE("reflect") {
        using math::Vector3;
        using math::reflect;

        static constexpr Vector3 a{1.0f, 2.0f, 3.0f};
        static constexpr Vector3 b{4.0f, 5.0f, 6.0f};

        RAC_CHECK_EQ(
            ( reflect(a, b) ),
            ( Vector3{-255.0f, -318.0f, -381.0f} )
        );
    }

    TEST_CASE("angle") {
        using math::Vector3;
        using math::angle;
        using math::PI;

        static constexpr Vector3 a{2.0f, 4.0f, 4.0f};
        static constexpr Vector3 b{-4.0f, -8.0f, -8.0f};

        R_CHECK_EQ(
            ( angle(a, b) ),
            ( PI )
        );
    }

    TEST_CASE("lerp") {
        using math::Vector3;
        using math::lerp;

        static constexpr Vector3 a{1.0f, 2.0f, 3.0f};
        static constexpr Vector3 b{4.0f, 5.0f, 6.0f};

        BASIC_RAC_SUBCASE("weight = -1",
            ( lerp(a, b, -1.0f) ),
            ( Vector3{-2.0f, -1.0f, -0.0f} )
        );

        BASIC_RAC_SUBCASE("weight = 0",
            ( lerp(a, b, 0.0f) ),
            ( a )
        );

        BASIC_RAC_SUBCASE("weight = 0.5",
            ( lerp(a, b, 0.5f) ),
            ( Vector3{2.5f, 3.5f, 4.5f} )
        );

        BASIC_RAC_SUBCASE("weight = 1",
            ( lerp(a, b, 1.0f) ),
            ( b )
        );

        BASIC_RAC_SUBCASE("weight = 2",
            ( lerp(a, b, 2.0f) ),
            ( Vector3{7.0f, 8.0f, 9.0f} )
        );
    }

    TEST_CASE("min") {
        using math::Vector3;
        using math::min;

        static constexpr Vector3 a{5.0f, 8.0f, 3.0f};
        static constexpr Vector3 b{1.0f, 6.0f, 7.0f};

        BASIC_RAC_SUBCASE("vector",
            ( min(a, b) ),
            ( Vector3{1.0f, 6.0f, 3.0f} )
        );

        static constexpr Vector3 expected{4.0f, 4.0f, 3.0f};

        BASIC_RAC_SUBCASE_2("float",
            ( min(a, 4.0f) ),
            ( expected ),
            ( min(4.0f, a) ),
            ( expected )
        );
    }

    TEST_CASE("min_length") {
        using math::Vector3;
        using math::length;
        using math::min_length;

        static constexpr Vector3 a{2.0f, 4.0f, 4.0f};   // len: 6
        static constexpr Vector3 b{5.0f, 10.0f, 10.0f}; // len: 15

        BASIC_RAC_SUBCASE("vector",
            ( min_length(a, b) ),
            ( a )
        );

        SUBCASE("float") {
            static constexpr f32 smaller_length = 1.0f;
            static constexpr f32 larger_length = 10.0f;

            const Vector3 result_smaller = min_length(a, smaller_length);
            const Vector3 result_swapped = min_length(smaller_length, a);
            const Vector3 result_larger = min_length(larger_length, a);

            CHECK_EQ(length(result_smaller), smaller_length);
            CHECK_EQ(result_smaller, result_swapped);
            CHECK_EQ(result_larger, a);
        }
    }

    TEST_CASE("max") {
        using math::Vector3;
        using math::max;

        static constexpr Vector3 a{5.0f, 8.0f, 3.0f};
        static constexpr Vector3 b{1.0f, 6.0f, 7.0f};

        BASIC_RAC_SUBCASE("vector",
            ( max(a, b) ),
            ( Vector3{5.0f, 8.0f, 7.0f} )
        );

        static constexpr Vector3 expected{5.0f, 8.0f, 4.0f};

        BASIC_RAC_SUBCASE_2("float",
            ( max(a, 4.0f) ),
            ( expected ),
            ( max(4.0f, a) ),
            ( expected )
        );
    }

    TEST_CASE("max_length") {
        using math::Vector3;
        using math::length;
        using math::max_length;

        static constexpr Vector3 a{2.0f, 4.0f, 4.0f};   // len: 6
        static constexpr Vector3 b{5.0f, 10.0f, 10.0f}; // len: 15

        BASIC_RAC_SUBCASE("vector",
            ( max_length(a, b) ),
            ( b )
        );

        SUBCASE("float") {
            static constexpr f32 smaller_length = 1.0f;
            static constexpr f32 larger_length = 10.0f;

            const Vector3 result_smaller = max_length(a, smaller_length);
            const Vector3 result_swapped = max_length(larger_length, a);
            const Vector3 result_larger = max_length(larger_length, a);

            CHECK_EQ(length(result_larger), larger_length);
            CHECK_EQ(result_larger, result_swapped);
            CHECK_EQ(result_smaller, a);
        }
    }

    TEST_CASE("clamp") {
        using math::Vector3;
        using math::clamp;

        static constexpr Vector3 a{5.0f, 8.0f, 3.0f};
        static constexpr Vector3 b{1.0f, 6.0f, 7.0f};
        static constexpr Vector3 c{3.0f, 9.0f, 8.0f};

        BASIC_RAC_SUBCASE("vector",
            ( clamp(a, b, c) ),
            ( Vector3{3.0f, 8.0f, 7.0f} )
        );

        BASIC_RAC_SUBCASE("float",
            ( clamp(a, 4.0f, 5.0f) ),
            ( Vector3{5.0f, 5.0f, 4.0f} )
        );
    }

    TEST_CASE("clamp_length") {
        using math::Vector3;
        using math::length;
        using math::clamp_length;

        static constexpr Vector3 v{2.0f, 4.0f, 4.0f}; // len: 6

        BASIC_R_SUBCASE("length < min",
            ( clamp_length(v, 12.0f, 14.0f) ),
            ( Vector3{4.0f, 8.0f, 8.0f} )
        );

        BASIC_R_SUBCASE("length == min",
            ( clamp_length(v, 6.0f, 9.0f) ),
            ( v )
        );

        BASIC_R_SUBCASE("length == max",
            ( clamp_length(v, 3.0f, 6.0f) ),
            ( v )
        );

        BASIC_R_SUBCASE("length > max",
            ( clamp_length(v, 1.0f, 3.0f) ),
            ( Vector3{1.0f, 2.0f, 2.0f} )
        );
    }

    TEST_CASE("abs") {
        using math::Vector3;
        using math::abs;

        RAC_CHECK_EQ(
            ( abs(Vector3{1.0f, -2.0f, 0.0f}) ),
            ( Vector3{1.0f, 2.0f, 0.0f} )
        );
    }

    TEST_CASE("rounding") {
        using math::Vector3;
        using math::floor;
        using math::ceil;
        using math::trunc;
        using math::round;
        
        static constexpr Vector3 a{0.5f, -0.5f, 1.4f};
        static constexpr Vector3 b{-1.0f, 0.0f, 1.0f};

        BASIC_RAC_SUBCASE_2("floor",
            ( floor(a) ),
            ( Vector3{0.0f, -1.0f, 1.0f} ),
            ( floor(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("ceil",
            ( ceil(a) ),
            ( Vector3{1.0f, 0.0f, 2.0f} ),
            ( ceil(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("trunc",
            ( trunc(a) ),
            ( Vector3{0.0f, 0.0f, 1.0f} ),
            ( trunc(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("round",
            ( round(a) ),
            ( Vector3{1.0f, -1.0f, 1.0f} ),
            ( round(b) ),
            ( b )
        );
    }

    TEST_CASE("sign") {
        using math::Vector3;
        using math::sign;

        RAC_CHECK_EQ(
            ( sign(Vector3{1.0f, -1.0f, 0.0f}) ),
            ( Vector3{1.0f, -1.0f, 0.0f} )
        );
    }

    TEST_CASE("approx_eq") {
        using math::Vector3;
        using math::approx_eq;
        using math::CMP_EPSILON;

        static constexpr Vector3 v{1.0f, 2.0f, 3.0f};
        static constexpr Vector3 offset = Vector3::xAxis(CMP_EPSILON);

        BASIC_R_SUBCASE("distance < epsilon",
            ( approx_eq(v, v + offset * 0.5f) ),
            ( true )
        );

        BASIC_R_SUBCASE("distance == epsilon",
            ( approx_eq(v, v + offset) ),
            ( true )
        );

        BASIC_R_SUBCASE("distance > epsilon",
            ( approx_eq(v, v + offset * 2.0f) ),
            ( false )
        );
    }

    TEST_CASE("cross") {
        using math::Vector3;
        using math::cross;

        RAC_CHECK_EQ(
            ( cross(Vector3::xAxis(), Vector3::yAxis()) ),
            ( Vector3::zAxis() )
        )
    }
}

TEST_SUITE("vector4") {
    TEST_CASE("constructors") {
        using math::Vector2;
        using math::Vector3;
        using math::Vector4;

        static constexpr Vector2 a{1.0f, 2.0f};
        static constexpr Vector3 b{3.0f, 4.0f, 5.0f};
        
        BASIC_RAC_SUBCASE("float",
            ( Vector4(1.0f) ),
            ( Vector4{1.0f, 1.0f, 1.0f, 1.0f} )
        );
        
        BASIC_RAC_SUBCASE("vec2, float, float",
            ( Vector4(a, 6.0f, 7.0f) ),
            ( Vector4{1.0f, 2.0f, 6.0f, 7.0f} )
        );

        BASIC_RAC_SUBCASE("float, vec2, float",
            ( Vector4(6.0f, a, 7.0f) ),
            ( Vector4{6.0f, 1.0f, 2.0f, 7.0f} )
        );

        BASIC_RAC_SUBCASE("float, float, vec2",
            ( Vector4(6.0f, 7.0f, a) ),
            ( Vector4{6.0f, 7.0f, 1.0f, 2.0f} )
        );

        BASIC_RAC_SUBCASE("vec2, vec2",
            ( Vector4(a, a) ),
            ( Vector4{1.0f, 2.0f, 1.0f, 2.0f} )
        );

        BASIC_RAC_SUBCASE("vec3, float",
            ( Vector4(b, 6.0f) ),
            ( Vector4{3.0f, 4.0f, 5.0f, 6.0f} )
        );

        BASIC_RAC_SUBCASE("float, vec3",
            ( Vector4(6.0f, b) ),
            ( Vector4{6.0f, 3.0f, 4.0f, 5.0f} )
        );

        BASIC_RAC_SUBCASE("vec2",
            ( Vector4(a) ),
            ( Vector4{1.0f, 2.0f, 0.0f, 0.0f} )
        );

        BASIC_RAC_SUBCASE("vec3",
            ( Vector4(b) ),
            ( Vector4{3.0f, 4.0f, 5.0f, 0.0f} )
        );
    }

    TEST_CASE("access") {
        using math::Vector4;

        static constexpr Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);

        RAC_CHECK_EQ(v[0], 1.0f);
        RAC_CHECK_EQ(v[1], 2.0f);
        RAC_CHECK_EQ(v[2], 3.0f);
        RAC_CHECK_EQ(v[3], 4.0f);
    }

    TEST_CASE("swizzle") {
        using math::Vector2;
        using math::Vector3;
        using math::Vector4;
        
        static constexpr Vector4 v{1.0f, 2.0f, 3.0f, 4.0f};

        BASIC_RAC_SUBCASE("vec2",
            ( v[1, 0] ),
            ( Vector2{2.0f, 1.0f} )
        );

        BASIC_RAC_SUBCASE("vec3",
            ( v[1, 2, 0] ),
            ( Vector3{2.0f, 3.0f, 1.0f} )
        );

        BASIC_RAC_SUBCASE("vec4",
            ( v[0, 2, 1, 3] ),
            ( Vector4{1.0f, 3.0f, 2.0f, 4.0f} )
        );
    }

    TEST_CASE("swap") {
        using math::Vector4;

        Vector4 a{1.f, 2.f, 3.f, 4.f};
        Vector4 b{4.f, 3.f, 2.f, 1.f};

        std::swap(a, b);

        CHECK_EQ(a, Vector4{4.f, 3.f, 2.f, 1.f});
        CHECK_EQ(b, Vector4{1.f, 2.f, 3.f, 4.f});
    }

    TEST_CASE("dot") {
        using math::Vector4;
        using math::dot;

        static constexpr Vector4 a{1.0f, 2.0f, 3.0f, 4.0f};
        static constexpr Vector4 b{5.0f, 6.0f, 7.0f, 8.0f};

        BASIC_RAC_SUBCASE("basic",
            ( dot(a, b) ),
            ( 70.0f )
        );

        BASIC_RAC_SUBCASE("self",
            ( dot(a, a) ),
            ( 30.0f )
        );

        BASIC_RAC_SUBCASE("zero",
            ( dot(a, Vector4()) ),
            ( 0.0f )
        );
    }

    TEST_CASE("length") {
        using math::Vector4;
        using math::length;
        using math::length_sq;

        static constexpr Vector4 v{1.0f, 2.0f, 2.0f, 4.0f};

        BASIC_R_SUBCASE("normal",
            ( length(v) ),
            ( 5.0f )
        );

        BASIC_RAC_SUBCASE("squared",
            ( length_sq(v) ),
            ( 25.0f )
        );
    }

    TEST_CASE("distance") {
        using math::Vector4;
        using math::distance;
        using math::distance_sq;

        static constexpr Vector4 a{1.0f, 2.0f, 2.0f, 4.0f};
        static constexpr Vector4 b{3.0f, 6.0f, 7.0f, 10.0f};

        BASIC_R_SUBCASE("normal",
            ( distance(a, b) ),
            ( 9.0f )
        );

        BASIC_RAC_SUBCASE("squared",
            ( distance_sq(a, b) ),
            ( 81.0f )
        );
    }

    TEST_CASE("normalize") {
        using math::Vector4;
        using math::length;
        using math::normalize;
        using math::normalize_fast;

        static constexpr Vector4 a{2.0f, 4.0f, 5.0f, 6.0f};
        static constexpr Vector4 b(1e-99);

        const Vector4 result = normalize(a);
        const Vector4 result_fast = normalize_fast(a);
        const Vector4 result_zero = normalize(b);

        CHECK_EQ(length(result), 1.0f);
        CHECK_EQ(result, result_fast);
        CHECK_EQ(result_zero, Vector4());
    }

    TEST_CASE("project") {
        using math::Vector4;
        using math::project;

        static constexpr Vector4 a{8.0f, 2.0f, 6.0f, 8.0f};
        static constexpr Vector4 b{12.0f, 14.0f, 8.0f, 6.0f};

        RAC_CHECK_EQ(
            ( project(a, b) ),
            ( Vector4{6.0f, 7.0f, 4.0f, 3.0f} )
        );
    }

    TEST_CASE("reflect") {
        using math::Vector4;
        using math::reflect;

        static constexpr Vector4 a{1.0f, 2.0f, 3.0f, 4.0f};
        static constexpr Vector4 b{5.0f, 6.0f, 7.0f, 8.0f};

        RAC_CHECK_EQ(
            ( reflect(a, b) ),
            ( Vector4{-699.0f, -838.0f, -977.0f, -1116.0f} )
        );
    }

    TEST_CASE("angle") {
        using math::Vector4;
        using math::angle;
        using math::PI2;

        static constexpr Vector4 a{1.0f, 5.0f, 1.0f, 3.0f};
        static constexpr Vector4 b{2.0f, -6.0f, -2.0f, 10.0f};

        R_CHECK_EQ(
            ( angle(a, b) ),
            ( PI2 )
        );
    }

    TEST_CASE("lerp") {
        using math::Vector4;
        using math::lerp;

        static constexpr Vector4 a{1.0f, 2.0f, 3.0f, 4.0f};
        static constexpr Vector4 b{5.0f, 6.0f, 7.0f, 8.0f};

        BASIC_RAC_SUBCASE("weight = -1",
            ( lerp(a, b, -1.0f) ),
            ( Vector4{-3.0f, -2.0f, -1.0f, 0.0f} )
        );

        BASIC_RAC_SUBCASE("weight = 0",
            ( lerp(a, b, 0.0f) ),
            ( a )
        );

        BASIC_RAC_SUBCASE("weight = 0.5",
            ( lerp(a, b, 0.5f) ),
            ( Vector4{3.0f, 4.0f, 5.0f, 6.0f} )
        );

        BASIC_RAC_SUBCASE("weight = 1",
            ( lerp(a, b, 1.0f) ),
            ( b )
        );

        BASIC_RAC_SUBCASE("weight = 2",
            ( lerp(a, b, 2.0f) ),
            ( Vector4{9.0f, 10.0f, 11.0f, 12.0f} )
        );
    }

    TEST_CASE("min") {
        using math::Vector4;
        using math::min;

        static constexpr Vector4 a{5.0f, 8.0f, 3.0f, 4.0f};
        static constexpr Vector4 b{1.0f, 6.0f, 7.0f, 2.0f};

        BASIC_RAC_SUBCASE("vector",
            ( min(a, b) ),
            ( Vector4{1.0f, 6.0f, 3.0f, 2.0f} )
        );

        static constexpr Vector4 expected{4.0f, 4.0f, 3.0f, 4.0f};

        BASIC_RAC_SUBCASE_2("float",
            ( min(a, 4.0f) ),
            ( expected ),
            ( min(4.0f, a) ),
            ( expected )
        );
    }

    TEST_CASE("min_length") {
        using math::Vector4;
        using math::length;
        using math::min_length;

        static constexpr Vector4 a{1.0f, 2.0f, 2.0f, 4.0f};   // len: 5
        static constexpr Vector4 b{1.0f, -3.0f, -1.0f, 5.0f}; // len: 6

        BASIC_RAC_SUBCASE("vector",
            ( min_length(a, b) ),
            ( a )
        );

        SUBCASE("float") {
            static constexpr f32 smaller_length = 1.0f;
            static constexpr f32 larger_length = 10.0f;

            const Vector4 result_smaller = min_length(a, smaller_length);
            const Vector4 result_swapped = min_length(smaller_length, a);
            const Vector4 result_larger = min_length(larger_length, a);

            CHECK_EQ(length(result_smaller), smaller_length);
            CHECK_EQ(result_smaller, result_swapped);
            CHECK_EQ(result_larger, a);
        }
    }

    TEST_CASE("max") {
        using math::Vector4;
        using math::max;

        static constexpr Vector4 a{5.0f, 8.0f, 3.0f, 4.0f};
        static constexpr Vector4 b{1.0f, 6.0f, 7.0f, 2.0f};

        BASIC_RAC_SUBCASE("vector",
            ( max(a, b) ),
            ( Vector4{5.0f, 8.0f, 7.0f, 4.0f} )
        );

        static constexpr Vector4 expected{5.0f, 8.0f, 4.0f, 4.0f};

        BASIC_RAC_SUBCASE_2("float",
            ( max(a, 4.0f) ),
            ( expected ),
            ( max(4.0f, a) ),
            ( expected )
        );
    }

    TEST_CASE("max_length") {
        using math::Vector4;
        using math::length;
        using math::max_length;

        static constexpr Vector4 a{1.0f, 2.0f, 2.0f, 4.0f};   // len: 5
        static constexpr Vector4 b{1.0f, -3.0f, -1.0f, 5.0f}; // len: 6

        BASIC_RAC_SUBCASE("vector",
            ( max_length(a, b) ),
            ( b )
        );

        SUBCASE("float") {
            static constexpr f32 smaller_length = 1.0f;
            static constexpr f32 larger_length = 10.0f;

            const Vector4 result_smaller = max_length(a, smaller_length);
            const Vector4 result_swapped = max_length(larger_length, a);
            const Vector4 result_larger = max_length(larger_length, a);

            CHECK_EQ(length(result_larger), larger_length);
            CHECK_EQ(result_larger, result_swapped);
            CHECK_EQ(result_smaller, a);
        }
    }

    TEST_CASE("clamp") {
        using math::Vector4;
        using math::clamp;

        static constexpr Vector4 a{5.0f, 8.0f, 3.0f, 4.0f};
        static constexpr Vector4 b{1.0f, 6.0f, 7.0f, 2.0f};
        static constexpr Vector4 c{3.0f, 9.0f, 8.0f, 4.0f};

        BASIC_RAC_SUBCASE("vector",
            ( clamp(a, b, c) ),
            ( Vector4{3.0f, 8.0f, 7.0f, 4.0f} )
        );

        BASIC_RAC_SUBCASE("float",
            ( clamp(a, 4.0f, 5.0f) ),
            ( Vector4{5.0f, 5.0f, 4.0f, 4.0f} )
        );
    }

    TEST_CASE("clamp_length") {
        using math::Vector4;
        using math::length;
        using math::clamp_length;

        static constexpr Vector4 v{1.0f, -3.0f, -1.0f, 5.0f}; // len: 6

        BASIC_R_SUBCASE("length < min",
            ( clamp_length(v, 12.0f, 14.0f) ),
            ( Vector4{2.0f, -6.0f, -2.0f, 10.0f} )
        );

        BASIC_R_SUBCASE("length == min",
            ( clamp_length(v, 6.0f, 9.0f) ),
            ( v )
        );

        BASIC_R_SUBCASE("length == max",
            ( clamp_length(v, 3.0f, 6.0f) ),
            ( v )
        );

        BASIC_R_SUBCASE("length > max",
            ( clamp_length(v, 1.0f, 3.0f) ),
            ( Vector4{0.5f, -1.5f, -0.5f, 2.5f} )
        );
    }

    TEST_CASE("abs") {
        using math::Vector4;
        using math::abs;

        RAC_CHECK_EQ(
            ( abs(Vector4{1.0f, -2.0f, -3.0f, 0.0f}) ),
            ( Vector4{1.0f, 2.0f, 3.0f, 0.0f} )
        );
    }

    TEST_CASE("rounding") {
        using math::Vector4;
        using math::floor;
        using math::ceil;
        using math::trunc;
        using math::round;
        
        static constexpr Vector4 a{0.5f, -0.5f, 1.4f, 1.6f};
        static constexpr Vector4 b{-1.0f, 0.0f, 1.0f, 2.0f};

        BASIC_RAC_SUBCASE_2("floor",
            ( floor(a) ),
            ( Vector4{0.0f, -1.0f, 1.0f, 1.0f} ),
            ( floor(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("ceil",
            ( ceil(a) ),
            ( Vector4{1.0f, 0.0f, 2.0f, 2.0f} ),
            ( ceil(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("trunc",
            ( trunc(a) ),
            ( Vector4{0.0f, 0.0f, 1.0f, 1.0f} ),
            ( trunc(b) ),
            ( b )
        );

        BASIC_RAC_SUBCASE_2("round",
            ( round(a) ),
            ( Vector4{1.0f, -1.0f, 1.0f, 2.0f} ),
            ( round(b) ),
            ( b )
        );
    }

    TEST_CASE("sign") {
        using math::Vector4;
        using math::sign;

        RAC_CHECK_EQ(
            ( sign(Vector4{1.0f, -1.0f, 0.0f, -0.0f}) ),
            ( Vector4{1.0f, -1.0f, 0.0f, 0.0f} )
        );
    }

    TEST_CASE("approx_eq") {
        using math::Vector4;
        using math::approx_eq;
        using math::CMP_EPSILON;

        static constexpr Vector4 v{1.0f, 2.0f, 3.0f, 4.0f};
        static constexpr Vector4 offset = Vector4::xAxis(CMP_EPSILON);

        BASIC_R_SUBCASE("distance < epsilon",
            ( approx_eq(v, v + offset * 0.5f) ),
            ( true )
        );

        BASIC_R_SUBCASE("distance == epsilon",
            ( approx_eq(v, v + offset) ),
            ( true )
        );

        BASIC_R_SUBCASE("distance > epsilon",
            ( approx_eq(v, v + offset * 2.0f) ),
            ( false )
        );
    }

    TEST_CASE("Transform") {
        using math::Transform;

        static constexpr Transform transform;
        STATIC_REQUIRE(transform.position[0] == 0.f);
        STATIC_REQUIRE(transform.position[1] == 0.f);
        STATIC_REQUIRE(transform.position[2] == 0.f);
        STATIC_REQUIRE(transform.rotation[0] == 0.f);
        STATIC_REQUIRE(transform.rotation[1] == 0.f);
        STATIC_REQUIRE(transform.rotation[2] == 0.f);
        STATIC_REQUIRE(transform.scale[0] == 1.f);
        STATIC_REQUIRE(transform.scale[1] == 1.f);
        STATIC_REQUIRE(transform.scale[2] == 1.f);

    }
}