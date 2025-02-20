#include "ObjectRefUtil.h"

#include "StringUtil.h"

namespace ObjectRefUtil {
    int ObjectRefUtil::getMotionType(RE::TESObjectREFR* object) {
        std::int32_t motionType = -1;

        if (!object) {
            return -1;
        }

        const auto root = object->Get3D();
        if (!root) {
            return -1;
        }

        const auto cell = object->GetParentCell();
        const auto world = cell ? cell->GetbhkWorld() : nullptr;

        if (world) {
            RE::BSReadLockGuard locker(world->worldLock);

            RE::BSVisit::TraverseScenegraphCollision(
                root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
                    const auto body = a_col->body.get();
                    const auto hkpRigidBody = body ? static_cast<RE::hkpRigidBody*>(body->referencedObject.get()) : nullptr;
                    if (hkpRigidBody) {
                        motionType = hkpRigidBody->motion.type.underlying();
                    }
                    return motionType != -1 ? RE::BSVisit::BSVisitControl::kStop : RE::BSVisit::BSVisitControl::kContinue;
                });
        }

        return motionType;
    }

    bool ObjectRefUtil::isInBBLS(RE::TESObjectREFR* object) {
        if (!object) {
            return false;
        }

        const auto root = object->Get3D();
        if (!root) {
            return false;
        }

        const auto cell = object->GetParentCell();
        std::string id = cell->GetFormEditorID();
        StringUtil::toLower(&id);
        return id.find("luxurysuite") != std::string::npos;
    }
}