#include <SFML/Graphics.hpp>
#include <array>
#include <random>

using namespace sf;

// Массив содержащий все спрайты
std::array <std::array<Sprite, 4>, 4> sprites;
// Размер текстуры спрайта
const int cube_widht = 63;
// Координаты свободной ячейки
int free_x = 3, free_y = 3;
void fEvent(RenderWindow& window);
void fSwapSpriteTextureRect(Sprite& sprite_1, Sprite& sprite_2);
void fMoveWithAnimation(Sprite& sprite_1, Sprite& sprite_2, RenderWindow& window, float x_move = 0, float y_move = 0, float speed = 3, int block_widht = cube_widht);
int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(252, 252), "Fifteen", Style::Default, settings);

	// Задаем максимальную частоту кадров (иначе эффект анимации может быть незаметен)
	window.setFramerateLimit(60);

	// Создаем переменную для текстуры и загружаем текстуру
	Texture texture_cube;
	texture_cube.loadFromFile("Cube.png");

	// Заполняем спрайты текстурой
	for (int x = 0; x < sprites.size(); x++)
		for (int y = 0; y < sprites.at(x).size(); y++)
		{
			sprites.at(x).at(y).setTexture(texture_cube);
			sprites.at(x).at(y).setTextureRect(IntRect(x * cube_widht, y * cube_widht, cube_widht, cube_widht));
			sprites.at(x).at(y).setPosition(x * cube_widht * 1.f, y * cube_widht * 1.f);
		}

	// Установим случайные начальные позиции
	for (int x = 0; x < sprites.size(); x++)
		for (int y = 0; y < sprites.at(x).size(); y++)
		{
			// Получим рандомные значения адреса спрайта в диапазоне {0-3}
			int x_rand = std::mt19937(std::random_device{}())() % 4;
			int y_rand = std::mt19937(std::random_device{}())() % 4;
			// Поменяем позиции спрайтов местами, пустую ячейку не трогаем
			if ((x != 3 || y != 3) && (x_rand != 3 || y_rand != 3))
				fSwapSpriteTextureRect(sprites.at(x).at(y), sprites.at(x_rand).at(y_rand));
		}

	// Основной цикл
	while (window.isOpen())
	{
		window.clear(Color::White);
		// Обработчик событий
		fEvent(window);

		for (int x = 0; x < sprites.size(); x++)
			for (int y = 0; y < sprites.at(x).size(); y++)
				window.draw(sprites.at(x).at(y));

		// Отрисовка окна
		window.display();
	}
	return 0;
}

// Обработчик событий
void fEvent(RenderWindow& window)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
		if (event.type == Event::MouseButtonPressed)
		{
			Vector2i pos = Mouse::getPosition(window);
			int x = pos.x / cube_widht;
			int y = pos.y / cube_widht;
			if (event.key.code == Mouse::Left)
			{
				// Проверяем можем ли мы переместить этот блок
				if (x - 1 == free_x && y == free_y)
				{
					// Перемещение без анимации
					//fSwapSpriteTextureRect(sprites.at(x).at(y), sprites.at(free_x).at(y));
					// Перемещение с анимацией (две строки)
					fMoveWithAnimation(sprites.at(x).at(y), sprites.at(free_x).at(y), window, -1);
					std::swap(sprites.at(x).at(y), sprites.at(free_x).at(y));
					free_x = x;
				}
				if (x + 1 == free_x && y == free_y)
				{
					// Перемещение без анимации
					//fSwapSpriteTextureRect(sprites.at(x).at(y), sprites.at(free_x).at(y));
					// Перемещение с анимацией (две строки)
					fMoveWithAnimation(sprites.at(x).at(y), sprites.at(free_x).at(y), window, 1);
					std::swap(sprites.at(x).at(y), sprites.at(free_x).at(y));
					free_x = x;
				}
				if (y - 1 == free_y && x == free_x)
				{
					// Перемещение без анимации
					//fSwapSpriteTextureRect(sprites.at(x).at(y), sprites.at(x).at(free_y));
					// Перемещение с анимацией (две строки)
					fMoveWithAnimation(sprites.at(x).at(y), sprites.at(x).at(free_y), window, 0, -1);
					std::swap(sprites.at(x).at(y), sprites.at(x).at(free_y));
					free_y = y;
				}
				if (y + 1 == free_y && x == free_x)
				{
					// Перемещение без анимации
					//fSwapSpriteTextureRect(sprites.at(x).at(y), sprites.at(x).at(free_y));
					// Перемещение с анимацией (две строки)
					fMoveWithAnimation(sprites.at(x).at(y), sprites.at(x).at(free_y), window, 0, 1);
					std::swap(sprites.at(x).at(y), sprites.at(x).at(free_y));
					free_y = y;
				}
			}
		}
	}
}

// Меняет местами текстуры двух спрайтов
void fSwapSpriteTextureRect(Sprite& sprite_1, Sprite& sprite_2)
{
	IntRect temp = sprite_1.getTextureRect();
	sprite_1.setTextureRect(sprite_2.getTextureRect());
	sprite_2.setTextureRect(temp);
}
// Двигаем спрайты с анимацией
void fMoveWithAnimation(Sprite& sprite_1, Sprite& sprite_2, RenderWindow& window, float x_move, float y_move, float speed, int block_widht)
{
	for (float i = 0; i < block_widht; i += speed)
	{
		// Двигаем первый спрайт
		sprite_1.move(speed * x_move, speed * y_move);
		// Если двигаем по оси X, то второй двигаем в другую сторону
		if (x_move != 0)
			sprite_2.move(speed * (-x_move), speed * y_move);
		// Если двигаем по оси Y, то второй двигаем в другую сторону
		if (y_move != 0)
			sprite_2.move(speed * x_move, speed * (-y_move));
		// Отрисовка спрайтов и окна
		for (int x = 0; x < sprites.size(); x++)
			for (int y = 0; y < sprites.at(x).size(); y++)
				window.draw(sprites.at(x).at(y));
		window.draw(sprite_1);
		window.display();
	}
}
