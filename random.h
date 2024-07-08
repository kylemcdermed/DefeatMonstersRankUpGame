#ifndef RANDOM.H
#define RANDOM.H

namespace Random {

    inline std::mt19937 generate() {

        std::random_device rd{};

        // Create seed_seq clock and 7 random numbers from std::random_device
        std::seed_seq ss {
            static_cast<std::seed_seq::result_type>
            (std::chrono::steady_clock::now().time_since_epoch().count()),rd(), rd(), rd() ,rd(), rd(), rd(), rd()};

            return std::mt19937{ss};
    }

    inline std::mt19937 mt{generate()};

    inline int get(int min, int max) {
        return std::uniform_int_distribution<int>{min,max}(mt);
    }


	template <typename T>
	T get(T min, T max) {
        return std::uniform_int_distribution<T>{min,max}(mt);
	}

	template <typename R, typename S, typename T>
	R get(S min, T max) {
        return get<R>(static_cast<R>(min), static_cast<R>(max));
    }

}

#endif // RANDOM
