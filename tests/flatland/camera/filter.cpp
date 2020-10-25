// TentFilter

class Filter {
public:
    Filter(const float radius) {}
    virtual float weight(const float x) const = 0;
};

class TentFilter : public Filter {
public:
    TentFilter(const float radius) {

    }
    float weight(const float x) const override {
        return 0;
    }
};

struct Sample {
    float x;
    float y;
    Color3f color;
};

class MockFilm {
public:
    void put(const Sample& s) {
        // where does sample contribute?
        // only to std::ceil(x) std::ceil(y)?
        // or in the whole radius?
    }
};

Color3f getPixelColor(float x, float y, int film_widht, int film_height) {
    // triangle.... sharp edge -> Filter result?
}

