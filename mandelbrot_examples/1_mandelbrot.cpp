#include <SFML/Graphics.hpp>
#include <complex>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITER = 1000;

sf::Color getColor(int iterations) {
    if (iterations == MAX_ITER) {
        return sf::Color::Black;
    }

    int r = (iterations * 9) % 256;
    int g = (iterations * 7) % 256;
    int b = (iterations * 5) % 256;
    return sf::Color(r, g, b);
}

int mandelbrot(const std::complex<double>& c) {
    std::complex<double> z = c;
    int n = 0;
    while (abs(z) <= 2.0 && n < MAX_ITER) {
        z = z * z + c;
        ++n;
    }
    return n;
}

void generateMandelbrot(sf::Image& image) {
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            double real = (x - WIDTH / 2.0) * 4.0 / WIDTH;
            double imag = (y - HEIGHT / 2.0) * 4.0 / HEIGHT;
            std::complex<double> c(real, imag);
            int iterations = mandelbrot(c);
            sf::Color color = getColor(iterations);
            image.setPixel(x, y, color);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    sf::Image image;
    image.create(WIDTH, HEIGHT);
    generateMandelbrot(image);

    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}