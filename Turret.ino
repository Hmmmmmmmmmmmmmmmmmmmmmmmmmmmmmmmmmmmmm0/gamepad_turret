#include <Servo.h>

// Определяем сервы
Servo servoX;  // Поворот (SG90-360)
Servo servoY;  // Наклон (SG90-360)
Servo servoFire;  // Выстрел (обычная SG90)

// Пины
const int servoXPin = 9;
const int servoYPin = 10;
const int servoFirePin = 11;

// Стоп сигнал
const int STOP_SIGNAL = 90;

// Виртуальные позиции
int posX = 0;
int posY = 0;

// Ограничения
const int limitX = 200;  // 2 оборота в каждую сторону
const int limitY = 90;   // 180 градусов в каждую сторону

// Порог для фильтрации слабых сигналов
const int DEAD_ZONE = 10;  // если значение меньше этого, не реагируем

void setup() {
  Serial.begin(115200);
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
  servoFire.attach(servoFirePin);

  servoX.write(STOP_SIGNAL);
  servoY.write(STOP_SIGNAL);
  servoFire.write(0);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    int x, y, fire;
    sscanf(data.c_str(), "%d,%d,%d", &x, &y, &fire);

    // Фильтрация слабых сигналов для оси X и Y
    if (abs(x) < DEAD_ZONE) x = 0;  // Если x слишком мал, не изменяем позицию
    if (abs(y) < DEAD_ZONE) y = 0;  // Если y слишком мал, не изменяем позицию

    // === Поворот по X ===
    int deltaX = x / 30;  // Чем больше стик — тем быстрее "вращение"
    posX += deltaX;

    if (posX > limitX) {
      posX = limitX;
      servoX.write(STOP_SIGNAL);
    } else if (posX < -limitX) {
      posX = -limitX;
      servoX.write(STOP_SIGNAL);
    } else {
      servoX.write(STOP_SIGNAL + x / 2);
    }

    // === Наклон по Y ===
    int deltaY = y / 30;
    posY += deltaY;

    if (posY > limitY) {
      posY = limitY;
      servoY.write(STOP_SIGNAL);
    } else if (posY < -limitY) {
      posY = -limitY;
      servoY.write(STOP_SIGNAL);
    } else {
      servoY.write(STOP_SIGNAL + y / 2);
    }

    // === Выстрел ===
    if (fire == 1) {
      servoFire.write(90);  // Усиленный удар
      delay(200);
      servoFire.write(0);
    }

    // === Отладка ===
    Serial.print("posX: ");
    Serial.print(posX);
    Serial.print(" posY: ");
    Serial.print(posY);
    Serial.print(" fire: ");
    Serial.println(fire);
  }
}
