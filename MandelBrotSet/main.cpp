#include <complex>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <thread>





using namespace std::literals;
typedef std::complex<double> complex;
typedef long double decimal;

inline int calcPixel(double cReal, double cImag, int maxIterr) {

	int intervall = 0;

	double zReal = 0;
	double zImag = 0;


	for (; zReal < 2; intervall++) {
		double temp = zReal * zReal - zImag * zImag + cReal;
		zImag = 2 * zReal * zImag + cImag;
		zReal = temp;

		/*real = z.real();
		imag = z.imag();
		z = complex(real * real - imag * imag,
					real * imag + imag * real);
		z += c;

		/*if (z == last1)
			return maxIterr;
		last1 = z;*/
		
		

		//z = z * z + c;

		if (intervall == maxIterr)
			return intervall;
	}
	return intervall;
}

inline void calcPart(sf::Image& img, decimal size, decimal posx, decimal y, int yy, int maxIterr, int ressx, int ressy) {
	int xx = 0;
	for (decimal x = -size / 2 + posx; x < size / 2 + posx; x += (size / (ressx - 5))) {

		unsigned int inter = calcPixel(x, y, maxIterr);

		unsigned char col = inter;
		int col1 = 0, col2 = 0, col3 = 0;
		if (col < 20) {
			col1 = 0;
			col2 = 0;
			col3 = col * 4;
		}
		else if (col < 40) {
			col1 = col;
			col2 = col / 2;
			col3 = col * (255 / 15);
		}
		else if (col < maxIterr * 0.5) {
			col1 = col * 2;
			col2 = col;
			col3 = col * (255 / 50);
		}
		else if (col < maxIterr * 0.7) {
			col1 = col * 2;
			col2 = col;
			col3 = 0;
		}
		else if (col < maxIterr * 0.97) {
			col1 = col / 2;
			col2 = col * 2;
			col3 = 0;
		}
		else {
			col1 = 0;
			col2 = 0; 
			col3 = 0;
		}

		img.setPixel(xx, yy, sf::Color(col1, col2, col3, 255));
		xx++;

	}
	
}

inline void calcThreadPart(sf::Image& img, decimal starty, decimal threadRange, decimal size, int i, decimal posx, int yy, int maxIterr, int ressx, int ressy) {
	for (decimal y = starty + threadRange * i;
		y < starty + threadRange * (i + 1); 
		y += (size / ((ressy - 5) * 2))) {
		calcPart(img, size, posx, y, yy, maxIterr, ressx, ressy);
		yy++;
	}
}


int main() {

	decimal size = 2;
	decimal posx = 0;
	decimal posy = 0;

	int maxIterr = 100;
	int ressx = 1350;
	int ressy = 1350 / 2;

	//std::cout << sizeof(custInt) << std::endl;
	
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text text;
	text.setFont(font);
	text.setPosition(20, 20);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);

	sf::RenderWindow window(sf::VideoMode(ressx, ressy), "Some Brot", sf::Style::Default);
	int threadCount = std::thread::hardware_concurrency();
	std::thread* threads = new std::thread[threadCount];
	sf::Image img;
	img.create(ressx + 25, ressy + 25);

	while (true) {
		
		
		
		int xx = 0;
		int yy = 0;

		
		bool change = false;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			posx -= 0.1 * size;
			change = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			posx += 0.1 * size;
			change = true;
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			posy -= 0.1 * size;
			change = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			posy += 0.1 * size;
			change = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			size *= 0.9;
			change = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			size /= 0.9;
			change = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
			maxIterr /= 0.9;
			change = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			maxIterr *= 0.9;
			change = true;
		}


		if (pow(size, -1) * 2 > 12'000'000'000'000) {
			size = pow(12'000'000'000'000, -1) * 2;
		}

		decimal starty = -size / 4 + posy;
		decimal endy = size / 4 + posy;

		decimal length = endy - starty;
		decimal threadRange = length / threadCount;

		
		if (change) {
			img.create(ressx + 25, ressy + 25);
			for (int i = 0; i < threadCount; i++) {
				yy = (ressy - 5) / threadCount * i;
				threads[i] = std::thread(calcThreadPart, std::ref(img), starty, threadRange, size, i, posx, yy, maxIterr, ressx, ressy);
			}

			for (int i = 0; i < threadCount; i++) {
				threads[i].join();
			}
		}

		sf::Event event;

		while (window.pollEvent(event)) {
			
		}
		

		sf::Texture tex;
		tex.loadFromImage(img);
		sf::Sprite spr(tex);
		spr.setScale(1.f, 1.f);
		window.clear();
		window.draw(spr);
		text.setString("Zoom: " + std::to_string(pow(size, -1) * 2));
		if(ressx > 999)
			window.draw(text);
		window.display();
	}

	




	return 0;
}