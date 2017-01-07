#include<SFML\Graphics.hpp>
#include<iostream>
#include<vector>
#include<cmath>

#define PI 3.14159265

int armCount = 0;

struct Arm {
	float length;
	float currentAngle;
	float rotateSpeed;

	sf::Vector2f rootPos;
};

sf::Vector2f getTrianglePoint(sf::Vector2f &pos, float &angle, float &lineLength);
void		 setupArmList	 (std::vector<Arm> *pArmList, sf::RenderWindow *pWin);
void		 createFigure	 (sf::VertexArray *vertexAry, std::vector<Arm> *armList, sf::RenderWindow *window);

int main()
{
	//VARS_OBJECTS
	sf::RenderWindow *window  = new sf::RenderWindow(sf::VideoMode(1000, 1000), "Spirograph");
	sf::Event		 *sfEvent = new sf::Event();
	sf::VertexArray   vertexAry(sf::LinesStrip, 0);
	std::vector<Arm>  armList;

	setupArmList(&armList,    window);
	createFigure(&vertexAry, &armList, window);

	//MAIN_LOOP
	while (window->isOpen()) {					//isOpen Loop
		while (window->pollEvent(*sfEvent)) {	//pollEvent Loop
			switch (sfEvent->type) {			//Event switch case 
			case sf::Event::Closed:				//Close button
				window->close();
				break;
			case sf::Event::KeyPressed:			//Esc button pressed
				if (sfEvent->key.code == sf::Keyboard::Escape)
					window->close();
				else if (sfEvent->key.code == sf::Keyboard::R) {
					setupArmList(&armList, window);
					createFigure(&vertexAry, &armList, window);
				}
				break;
			}
		}
		window->clear();

		window->draw(vertexAry);

		window->display();
	}

	delete(window);
	delete(sfEvent);

	return 0;
}

sf::Vector2f getTrianglePoint(sf::Vector2f &pos, float &angle, float &lineLength)
{
	sf::Vector2f _pointPos(0.F, 0.F);

	_pointPos.x = pos.x + std::cos(angle * PI / 180)  * lineLength;
	_pointPos.y = pos.y + std::sin(angle * PI / 180)  * lineLength;

	return _pointPos;
}

void setupArmList(std::vector<Arm> *pArmList, sf::RenderWindow *pWin)
{	
	Arm	tempArm;

	std::cout << ">How many arms? ";
	std::cin >> armCount;

	pArmList->clear();

	for (int i = 0; i < armCount; i++) {
		std::cout << ">Arm<" << i << ">.length      = ";
		std::cin >> tempArm.length;
		std::cout << ">Arm<" << i << ">.rotateSpeed = ";
		std::cin >> tempArm.rotateSpeed;
		tempArm.currentAngle = 0.F;
		if (i == 0) {
			tempArm.rootPos = sf::Vector2f(pWin->getSize().x / 2, pWin->getSize().y / 2);
		}
		else {
			tempArm.rootPos = sf::Vector2f(pWin->getSize().x / 2 + pArmList->at(i - 1).length, pWin->getSize().y / 2);
		}
		pArmList->push_back(tempArm);
	}
}
void createFigure(sf::VertexArray *vertexAry, std::vector<Arm> *armList, sf::RenderWindow *window)
{
	vertexAry->resize(0);

	for (int i = 0; i <= (720 * armCount); i++) {
		vertexAry->resize(vertexAry->getVertexCount() + 1);

		armList->at(0).rootPos = getTrianglePoint(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), armList->at(0).currentAngle, armList->at(0).length);
		armList->at(0).currentAngle += armList->at(0).rotateSpeed;
		for (int x = 1; x < armList->size(); x++) {
			armList->at(x).rootPos = getTrianglePoint(sf::Vector2f(armList->at(x - 1).rootPos), armList->at(x).currentAngle, armList->at(x).length);
			armList->at(x).currentAngle += armList->at(x).rotateSpeed;
		}
		(*vertexAry)[i].position = getTrianglePoint(sf::Vector2f(armList->at(armList->size() - 1).rootPos),
			armList->at(armList->size() - 1).currentAngle,
			armList->at(armList->size() - 1).length);

	}
}
