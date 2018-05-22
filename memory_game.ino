/* Pin configuration */

const int led[] = {7, 8, 9, 10};
const int button[] = {2, 3, 4, 5};
const int buzzer = 12;

void setup() {
  randomSeed(analogRead(0));
  pinMode(buzzer, OUTPUT);
  for(int i = 0; i < 4; i++){
    pinMode(button[i], INPUT);
    pinMode(led[i], OUTPUT);
  }
  // Set initial game state
  restart();
}

/* Game Configuration */

// Length of each pattern before advancing a level
const int pattern_length = 5;
// Initial speed of displaying the pattern
const int pattern_initial_speed = 500;
// The rate at which the difficulty increases every level
const int pattern_speed_step = 25;
// Maximum amount of time to wait for a button press before loosing the game
const int button_wait = 1000;
// The tone to play when you loose a level
const int loose_tone = 1000;


/* Tracking Current Game State */

// Current speed of the game
int pattern_speed;
// Store the current generated pattern
int pattern[pattern_length];
// How much of the pattern are we working with?
int pattern_displayed;


/* Helper functions */

// Play a tone out of the speaker
void playTone(int t, int duration) {
  for(long i = 0; i < duration * 1000L; i += t * 2) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(t);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(t);
  }
}

// Restart the game
void restart() {
  pattern_speed = pattern_initial_speed;
  pattern_displayed = 0;
  // Generate the first pattern
  gen_pattern();
}

// Generate a fresh pattern
void gen_pattern() {
  for(int i = 0; i < pattern_length; i++) {
    pattern[i] = random(0,4);
  }
}

// Flash all the lights
void light_all(int duration) {  
  for(int i = 0; i < 4; i++) {
    digitalWrite(led[i], HIGH);
  }
  delay(duration);
  for(int i = 0; i < 4; i++) {
    digitalWrite(led[i], LOW);
  }
  delay(duration);
}

// Increase the difficulty and signal to the user that
// the difficulty is increasing
void level_up() {
  pattern_displayed = 1;
  gen_pattern();
  pattern_speed = pattern_speed - pattern_speed_step;
  for(int i = 0; i < 2; i++) {
    light_all(pattern_speed);
  }
}

// Flash a light
void light(int i, int duration) {
  digitalWrite(led[i], HIGH);
  delay(duration);
  digitalWrite(led[i], LOW);
  delay(duration);
}

// Incorrect input :-(
void loose() {
  playTone(loose_tone, 100);
  restart();
}


void loop() {
  pattern_displayed = pattern_displayed + 1;
  if(pattern_displayed > pattern_length) {
    level_up();
  }
  for(int i = 0; i < pattern_displayed; i++) {
    light(pattern[i], pattern_speed);
  }
  light_all(pattern_speed);

  // User's turn!
  for(int i = 0; i < pattern_displayed; i++) {
    bool pressed = false;
    while(!pressed) {
      for(int j = 0; j < 4; j++) {
        if(digitalRead(button[j]) == HIGH) {
          if(pattern[i] != j) {
            loose();
          }
          digitalWrite(led[j], HIGH);
          delay(200);
          while(digitalRead(button[j]) == HIGH) {}
          digitalWrite(led[j], LOW);
          pressed = true;
        }
      }
    }
  }
  light_all(pattern_speed);
}
