#include "tween.h"

void Tween::Update(const SkyTime& time) {
    auto it = tweens.begin();
    while (it != tweens.end()) {
        auto* tween = *it;
        constexpr float ERROR_MARGIN {0.001f};
        if (*tween->value >= tween->goal - ERROR_MARGIN) {
            std::invoke(tween->oncomplete);
            it = tweens.erase(it);
        } else {
            tween->percent = tween->currSeconds / 
                    ( tween->timeSeconds == 0 
                    ? 0.0001f
                    : tween->timeSeconds);

            tween->currSeconds += time.dt;

            auto p = tween->percent;

            auto interp = [tween](float p, InterpolationTypes type) {
                switch (type) {
                    case QUAD: return p * p; break;
                    case CUBIC: return p * p * p; break;
                    case QUART: return p * p * p * p; break;
                    case QUINT: return p * p * p * p * p; break;
                    case EXPO: return powf(2, (10 * (p - 1))); break;
                    case SINE: return -cosf(p * (3.1415926f * 0.5f)) + 1; break;
                    case CIRC: return -(sqrtf(1 - (p * p)) - 1); break; 
                    case BACK: return p * p * (2.7f * p - 1.7f); break;
                    case ELASTIC: return -(powf(2,(10 * (p - 1)) * sinf((p - 1.075f) * (3.1415926f * 2.f) / 0.3f))); break;
                    default: break;
                }
                return p;
            };

            if (tween->type_in != LINEAR && tween->type_out != LINEAR) {
                p *= 2.f;
                if (p < 1.0f) { p = 0.5f * interp(p, tween->type_in); }
                else {
                    p = 2 - p;
                    p = 0.5f * (1 - interp(p, tween->type_out)) + 0.5;
                }
            } else if (tween->type_in == LINEAR && tween->type_out != LINEAR) {
                p = 1 - p;
                p = 1 - interp(p, tween->type_out);
            } else {
                p = interp(p, tween->type_in);
            }

            auto v = Interpolation::Lerp(
                    tween->start,
                    tween->goal,
                    p); 

            *tween->value = v;

            ++it;
        }
    }
}
