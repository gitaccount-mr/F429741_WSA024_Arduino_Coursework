#include <math.h>

// ─────────────────────────────────────────────────────────────
// 📌 Constants and Configuration
// ─────────────────────────────────────────────────────────────
const int B = 4250;
const long R0 = 100000L;
const int pinTempSensor = A0;

// Sampling configuration
float samplingRateHz = 1.0;
unsigned long sampleIntervalMs = 1000 / samplingRateHz;
const int CYCLE_DURATION_SECONDS = 60;
int numSamples = samplingRateHz * CYCLE_DURATION_SECONDS;
const int MAX_SAMPLES = 240;  // Max at 4Hz * 60s

// Data storage
float temperatureData[MAX_SAMPLES];

// Moving average configuration
const int MOVING_AVG_WINDOW = 10;
float variationHistory[MOVING_AVG_WINDOW];
int variationIndex = 0;

// Power modes
#define ACTIVE 0
#define IDLE 1
#define POWER_DOWN 2
int currentMode = ACTIVE;
int idleCycleCount = 0;

// Serial alias
#define debug Serial

// ─────────────────────────────────────────────────────────────
// 🔧 Setup
// ─────────────────────────────────────────────────────────────
void setup() {
  debug.begin(9600);
  while (!debug);

  debug.println(F("--------------------------------------------------------------"));
  debug.println(F("📊 Adaptive Temperature Monitoring System - Serial Output"));
  debug.println(F("--------------------------------------------------------------"));
  debug.println(F("Time (s)\tTemp (°C)\tFreq (Hz)\tMagnitude"));
}

// ─────────────────────────────────────────────────────────────
// 🔁 Main Loop
// ─────────────────────────────────────────────────────────────
void loop() {
  collect_temperature_data();
  float dominantFreq = apply_dft_and_output(temperatureData, numSamples, samplingRateHz);
  float totalVariation = compute_variation(temperatureData, numSamples);
  float predictedVariation = update_moving_average(totalVariation);
  currentMode = decide_power_mode(predictedVariation);

  // Adjust sampling based on frequency analysis (Nyquist principle)
  samplingRateHz = constrain(dominantFreq * 2.0, 0.5, 4.0);
  sampleIntervalMs = 1000 / samplingRateHz;
  numSamples = min(int(samplingRateHz * CYCLE_DURATION_SECONDS), MAX_SAMPLES);

  // Summary log
  debug.println();
  debug.println(F("--------------------------------------------------------------"));
  debug.println(F("🔄 Cycle Summary"));
  debug.print(F("📈 Predicted Variation: ")); debug.print(predictedVariation, 2); debug.println(F(" °C"));
  debug.print(F("🎯 Dominant Frequency : ")); debug.print(dominantFreq, 2); debug.println(F(" Hz"));
  debug.print(F("📏 New Sampling Rate  : ")); debug.print(samplingRateHz, 2); debug.println(F(" Hz"));
  debug.print(F("⚡ Power Mode         : "));
  if (currentMode == ACTIVE) debug.println(F("ACTIVE"));
  else if (currentMode == IDLE) debug.println(F("IDLE"));
  else debug.println(F("POWER DOWN"));
  debug.println("--------------------------------------------------------------");
  debug.println();

  delay(500);
}

// ─────────────────────────────────────────────────────────────
// Temperature Acquisition
// ─────────────────────────────────────────────────────────────
void collect_temperature_data() {
  const float invT0 = 1.0f / 298.15f;  // Reference temperature (Kelvin)

  for (int i = 0; i < numSamples; i++) {
    int a = analogRead(pinTempSensor);
    float tempC = NAN;

    if (a > 0 && a < 1023) {
      float R = R0 * (1023.0f / a - 1.0f);
      float tempK = 1.0f / (log(R / R0) / B + invT0);
      tempC = tempK - 273.15f;
    }

    temperatureData[i] = tempC;
    float time = i / samplingRateHz;

    // Output formatted data
    debug.print(time, 2);      debug.print("\t\t");
    debug.print(tempC, 2);     debug.print("\t\t");
    debug.print("-\t\t");      debug.println("-");
    
    delay(sampleIntervalMs);
  }
}

// ─────────────────────────────────────────────────────────────
// Frequency Analysis (DFT)
// ─────────────────────────────────────────────────────────────
float apply_dft_and_output(float* data, int N, float fs) {
  float maxMagnitude = 0;
  float dominantFreq = 0;

  for (int k = 1; k < N / 2; k++) {  // Skip DC component
    float real = 0, imag = 0;

    for (int n = 0; n < N; n++) {
      float angle = 2.0 * PI * k * n / N;
      real += data[n] * cos(angle);
      imag -= data[n] * sin(angle);
    }

    float magnitude = sqrt(real * real + imag * imag);
    float freq = (float)k * fs / N;

    // Output frequency domain data
    debug.print("-\t\t");
    debug.print("-\t\t");
    debug.print(freq, 4);  debug.print("\t\t");
    debug.println(magnitude, 4);

    if (magnitude > maxMagnitude) {
      maxMagnitude = magnitude;
      dominantFreq = freq;
    }
  }

  return dominantFreq;
}

// ─────────────────────────────────────────────────────────────
// Total Temperature Variation
// ─────────────────────────────────────────────────────────────
float compute_variation(float* data, int N) {
  float variation = 0.0;
  for (int i = 1; i < N; i++) {
    variation += abs(data[i] - data[i - 1]);
  }
  return variation;
}

// ─────────────────────────────────────────────────────────────
// Moving Average for Prediction
// ─────────────────────────────────────────────────────────────
float update_moving_average(float newVal) {
  variationHistory[variationIndex] = newVal;
  variationIndex = (variationIndex + 1) % MOVING_AVG_WINDOW;

  float sum = 0.0;
  for (int i = 0; i < MOVING_AVG_WINDOW; i++) {
    sum += variationHistory[i];
  }

  return sum / MOVING_AVG_WINDOW;
}

// ─────────────────────────────────────────────────────────────
// Power Mode Decision Logic
// ─────────────────────────────────────────────────────────────
int decide_power_mode(float predictedVariation) {
  if (predictedVariation > 2.0) {
    idleCycleCount = 0;
    return ACTIVE;
  } else if (predictedVariation > 0.5) {
    idleCycleCount = 0;
    return IDLE;
  } else {
    idleCycleCount++;
    return (idleCycleCount >= 5) ? POWER_DOWN : IDLE;
  }
}