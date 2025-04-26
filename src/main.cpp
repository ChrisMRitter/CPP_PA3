// Contains the main game loop and handles all repeating logic
#include "enemyManager.h"
#include "laser.h"
#include "player.h"
#include "textManager.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

static const float VIEW_WIDTH = 800.f;
static const float VIEW_HEIGHT = 600.f;

void startScreen(sf::RenderWindow &window, const sf::Font &font);
void checkPlayerHealth(Player &player, sf::RenderWindow &window,
                       const sf::Font &font);
void handleEvents(sf::RenderWindow &window);

int main() {
  sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "Space Shooter");
  window.setFramerateLimit(60);

  sf::Texture sheet;
  if (!sheet.loadFromFile("./Assets/Sprites/spaceship_spritesheet.png"))
    return -1;
  sf::Image sheetImg = sheet.copyToImage();
  sf::Texture chaserTex, turretTex;
  chaserTex.loadFromImage(sheetImg, sf::IntRect(0, 0, 64, 64));
  turretTex.loadFromImage(sheetImg, sf::IntRect(0, 256, 64, 64));
  auto chaserPtr = std::make_shared<sf::Texture>(chaserTex);
  auto turretPtr = std::make_shared<sf::Texture>(turretTex);

  Player player("./Assets/Sprites/hero spaceship.png", 300.f);
  std::vector<Laser> lasers;

  sf::Font font;
  if (!font.loadFromFile("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf"))
    return -1;
  textManager textMgr;
  textMgr.loadFont("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf");

  startScreen(window, font);

  EnemyManager chasers;
  chasers.configure(chaserPtr, 12, 2, 2.5f, 120.f, 1.0f);
  EnemyManager turrets;
  turrets.configure(turretPtr, 6, 10, 8.0f, 30.f, 5.0f);

  sf::Texture bgTex;
  if (!bgTex.loadFromFile("./Assets/Sprites/Planets and Space.png"))
    std::cerr << "Failed to load background\n";
  sf::Sprite background(bgTex);
  {
    auto ws = window.getSize(); auto ts = bgTex.getSize();
    background.setScale(ws.x/float(ts.x), ws.y/float(ts.y));
  }

  sf::Music bgMusic;
  if (bgMusic.openFromFile("./Assets/Sound/bgmusic.mp3")) {
    bgMusic.setLoop(true);
    bgMusic.setVolume(50.f);
    bgMusic.play();
  }

  sf::Clock clock;
  while (window.isOpen()) {
    handleEvents(window);
    checkPlayerHealth(player, window, font);

    float dt = clock.restart().asSeconds();
    if (dt > 0.1f) dt = 0.1f;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    player.update(dt, lasers, mousePos);

    for (auto &laser : lasers) laser.update(dt);

    sf::Vector2f ppos = player.getPosition();
    chasers.update(dt, ppos);
    turrets.update(dt, ppos);

    int chaserKills = chasers.handleLaserCollisions(lasers);
    textMgr.addScore(chaserKills * 100);
    int turretKills = turrets.handleLaserCollisions(lasers);
    textMgr.addScore(turretKills * 500);

    chasers.handlePlayerCollisions(player, 1);
    turrets.handlePlayerCollisions(player, 5);

    sf::Vector2f vc = window.getView().getCenter();
    sf::Vector2f vs = window.getView().getSize();
    textMgr.updatePlayerHealth(player.getHealth(), vc, vs);
    textMgr.updateScoreDisplay(vc, vs);

    window.clear();
    window.draw(background);
    chasers.draw(window);
    turrets.draw(window);
    player.draw(window);
    for (auto &laser : lasers) laser.draw(window);
    textMgr.draw(window);
    window.display();
  }

  return 0;
}

void startScreen(sf::RenderWindow &window, const sf::Font &font) {
  sf::Text prompt("Press Enter to Start", font, 50);
  prompt.setFillColor(sf::Color::White);
  prompt.setPosition(window.getSize().x/2.f - prompt.getGlobalBounds().width/2.f,
                     window.getSize().y/2.f - prompt.getGlobalBounds().height/2.f);
  while (window.isOpen()) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed) window.close();
      else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) return;
    }
    window.clear(); window.draw(prompt); window.display();
  }
}

void checkPlayerHealth(Player &player, sf::RenderWindow &window, const sf::Font &font) {
  if (player.getHealth() <= 0) {
    sf::Text gameOver("Game Over\nPress Esc to Quit", font, 50);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setPosition(window.getSize().x/2.f - gameOver.getGlobalBounds().width/2.f,
                         window.getSize().y/2.f - gameOver.getGlobalBounds().height/2.f);
    while (window.isOpen()) {
      sf::Event ev;
      while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) window.close();
        else if (ev.type==sf::Event::KeyPressed && ev.key.code==sf::Keyboard::Escape) window.close();
      }
      window.clear(); window.draw(gameOver); window.display();
    }
  }
}

void handleEvents(sf::RenderWindow &window) {
  sf::Event ev;
  while (window.pollEvent(ev)) {
    if (ev.type == sf::Event::Closed) window.close();
    else if (ev.type == sf::Event::Resized) {
      float winA = float(ev.size.width)/ev.size.height;
      float viewA = VIEW_WIDTH/VIEW_HEIGHT;
      sf::View v = window.getView();
      if (winA>viewA) v.setSize(VIEW_HEIGHT*winA,VIEW_HEIGHT);
      else            v.setSize(VIEW_WIDTH,VIEW_WIDTH/winA);
      window.setView(v);
    }
  }
}
