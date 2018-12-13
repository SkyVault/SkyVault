#include "tween.h"

void Tween::Update(const SkyTime& time) {
    auto it = tweens.begin();
    while (it != tweens.end()) {
        auto* tween = *it;
        if (*tween->value >= tween->goal) {
            std::invoke(tween->oncomplete);
            it = tweens.erase(it);
        } else {
            tween->percent = tween->currSeconds / 
                    ( tween->timeSeconds == 0 
                    ? 0.0001f
                    : tween->timeSeconds);

            tween->currSeconds += time.dt;

            auto p = tween->percent;

#if 0
p = p * 2
if p < 1 then
  return .5 * ($e)
else
  p = 2 - p
  return .5 * (1 - ($e)) + .5
end
#endif

            auto interp = [tween](float p, InterpolationTypes type) {
                switch (type) {
                    case QUAD: return p * p; break;
                    case CUBIC: return p * p * p; break;
                    case QUART: return p * p * p * p; break;
                    case QUINT: return p * p * p * p * p; break;
                    case EXPO: return powf(2, (10 * (p - 1))); break;
                    case SINE: return -cosf(p * (3.1415926 * 0.5f)) + 1; break;
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

            if (v > tween->goal) {
                *tween->value = tween->goal;
            } else { 
                *tween->value = v;
            }

            ++it;
        }
    }
}
