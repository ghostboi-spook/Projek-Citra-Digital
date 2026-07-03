# Heterogeneous Haze Visualization using Perlin Noise & Dark Channel Prior

An interactive, high-performance desktop application built from scratch in C++ to simulate and visualize realistic, heterogeneous haze effects on digital images. This project is a code replication and implementation based on the scientific paper:  
> *"An Environmental Domain Awareness for Serious-Game Using Perlin Noise Base Heterogeneous Haze Visualization"*

---

## 📌 Project Overview
While standard fog generation often applies uniform filters, this software extracts domain data from real-world images to generate physically plausible and stochastic haze environments.

## 🚀 Key Features
* **Real-time Image Manipulation:** Low-level pixel and texture rendering handled directly in hardware using **Raylib**.
* **Depth & Haze Estimation:** Implements the **Dark Channel Prior (DCP)** algorithm to analyze and extract haze density and location from real-world photography.
* **Stochastic Fog Generation:** Integrates **Perlin Noise** mathematically to create natural, non-uniform (heterogeneous) haze structures rather than flat overlays.

## 🛠️ Tech Stack & Tools
* **Language:** C++ (GCC/Clang)
* **Graphics Library:** Raylib
* **Algorithm Concepts:** Dark Channel Prior, Perlin Noise (Procedural Generation), Digital Image Processing (DIP).
