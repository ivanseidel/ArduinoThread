/*
  StaticThreadControllerLite.h - Controlls a list of ThreadsLite with different timings
  
  Basicaly, what it does is to keep track of current ThreadsLite and run when
  necessary.
  
  StaticThreadControllerLite is based on the class StaticThreadController,
  but takes less memory and maximum period is only 32,767 seconds.
  WARNING! It is not recommended to use if you do not have a memory deficit.
  
  Copyright © 2016 Alex Eremin.
  Copyright © 2018 Stanislav Hnatiuk.
  Released into the public domain.
*/

#ifndef STATICTHREADCONTROLLERLITE_H
#define STATICTHREADCONTROLLERLITE_H

#include "ThreadLite.h"

template <uint8_t N>
class StaticThreadControllerLite {
  protected:
    ThreadLite thread[N];
  public:
    template <typename... T>
    StaticThreadControllerLite(T... params) : thread{params...} {	};

    // Runs all ThreadLite
    void run() {
      for (uint8_t i = 0; i < N; i++) {
        // Is enabled? Timeout exceeded?
        if (thread[i].shouldRun()) {
          thread[i].run();
        }
      }
    }

    // Return the quantity of ThreadsLite
    static constexpr int size() {
      return N;
    };

    // Return the I ThreadLite on the array
    // Doesn't perform any bounds checks and behaviour is
    // unpredictable in case of index > N
    ThreadLite& operator[](uint8_t index) {
      return thread[index];
    }
};

#endif // STATICTHREADCONTROLLERLITE_H
