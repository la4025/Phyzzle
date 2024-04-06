#pragma once
#include <cstdint>
#include <Eigen/Dense>


namespace ZonaiPhysics
 {
    struct ZnContact
    {
        Eigen::Vector3f point{ Eigen::Vector3f::Zero() };         // 접촉점
        Eigen::Vector3f normal{ Eigen::Vector3f::Zero() };        // 접촉점의 노말
        Eigen::Vector3f impulse{ Eigen::Vector3f::Zero() };       // 접촉점에서의 충격량
        float separation{ 0.f };                                    // 침투 깊이
    };

	struct ZnCollision
 	{
        ZnCollision() = default;
        ~ZnCollision() = default;

        Eigen::Vector3f impulses{ Eigen::Vector3f::Zero() };           // 충격량 총량

        // Eigen::Vector3f thisPreLinearVelocity{ Eigen::Vector3f::Zero() };
        // Eigen::Vector3f otherPreLinearVelocity{ Eigen::Vector3f::Zero() };
        // Eigen::Vector3f thisPreAngularVelocity{ Eigen::Vector3f::Zero() };
        // Eigen::Vector3f otherPreAngularVelocity{ Eigen::Vector3f::Zero() };

        Eigen::Vector3f thisPostLinearVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f otherPostLinearVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f thisPostAngularVelocity{ Eigen::Vector3f::Zero() };
        Eigen::Vector3f otherPostAngularVelocity{ Eigen::Vector3f::Zero() };

        ZnContact* contacts{ nullptr };                // 접촉점 배열 포인터
        uint32_t contactCount{ 0 };              // 접촉점 개수
 	};
 }
