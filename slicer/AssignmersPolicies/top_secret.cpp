#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

// ANSI color helper
std::string rgbToAnsi(int r, int g, int b, bool foreground = true)
{
    return "\033[" + std::to_string(foreground ? 38 : 48) + ";2;" +
           std::to_string(r) + ";" + std::to_string(g) + ";" +
           std::to_string(b) + "m";
}

// HSV to RGB (for smooth gradients)
void hsvToRgb(float h, float s, float v, int &r, int &g, int &b)
{
    h = fmod(h, 360.0f);
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r_, g_, b_;
    if (h < 60)
    {
        r_ = c;
        g_ = x;
        b_ = 0;
    }
    else if (h < 120)
    {
        r_ = x;
        g_ = c;
        b_ = 0;
    }
    else if (h < 180)
    {
        r_ = 0;
        g_ = c;
        b_ = x;
    }
    else if (h < 240)
    {
        r_ = 0;
        g_ = x;
        b_ = c;
    }
    else if (h < 300)
    {
        r_ = x;
        g_ = 0;
        b_ = c;
    }
    else
    {
        r_ = c;
        g_ = 0;
        b_ = x;
    }

    r = static_cast<int>((r_ + m) * 255);
    g = static_cast<int>((g_ + m) * 255);
    b = static_cast<int>((b_ + m) * 255);
}

// Print text with gradient
void gradPrint(const std::string &text, float hueStart = 0.0f, float hueStep = 5.0f, int delayMs = 30)
{
    float hue = hueStart;
    for (char c : text)
    {
        int r, g, b;
        hsvToRgb(hue, 1.0f, 1.0f, r, g, b);
        std::cout << rgbToAnsi(r, g, b) << c << std::flush;
        hue = fmod(hue + hueStep, 360.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    std::cout << "\033[0m"; // Reset
}
// ANSI colors
const std::vector<std::string> COLORS = {
    "\033[31m", "\033[32m", "\033[33m", "\033[34m",
    "\033[35m", "\033[36m", "\033[91m", "\033[92m",
    "\033[93m", "\033[94m", "\033[95m", "\033[96m"};
const std::string COLOR_RESET = "\033[0m";

// Random color generator
std::string getRandomColor()
{
    volatile static int ind = 0;
    ind = (ind + 1) % COLORS.size();
    return COLORS[ind];
}

// Slow print with random colors
void hypePrint(const std::string &text, int delayMs = 30)
{
    for (char c : text)
    {
        std::cout << getRandomColor() << c << COLOR_RESET << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

// ASCII art fireworks
void printFirework()
{
    std::vector<std::string> firework = {
        "    .* *.    ",
        "   *  *  *   ",
        ".*   * *   *.",
        "   *  *  *   ",
        "    * * *    ",
        "     *.*     ",
        "      *      "};
    for (const auto &line : firework)
    {
        std::cout << getRandomColor() << line << COLOR_RESET << std::endl;
    }
}

int main()
{
    srand(time(0)); // Seed randomness

    // ==== PHASE 1: THE SURVIVOR'S DECLARATION ====
    gradPrint("\n\n\n***** FINAL SEMINAR OF THE YEAR *****\n");
    gradPrint("\nTEACHER: \"I HAVE SURVIVED....\"\n");
    gradPrint("\nSTUDENTS: COOLEST\n\n");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // ==== PHASE 2: FIREWORKS EXPLOSION ====
    for (int i = 0; i < 3; i++)
    {
        printFirework();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        system("clear || cls"); // Clear terminal (cross-platform)
    }

    // ==== PHASE 3: FAKE ERROR (FOR DRAMA) ====
    gradPrint("\n\nI'VE DELETED THE ASSESSMENT TABLE\n");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    gradPrint("...JUST KIDDING.\n\n");

    // ==== PHASE 4: SCROLLING CREDITS ====
    std::vector<std::string> credits = {
        "CREDITS:",
        "----------------",
        "STUDENTS: *those creatures who read, read and will read articles*",
        "SUFFERING: immeasurable",
        "ENGLISH CONSUMED: enough to knock down a horse",
        "----------------",
        "THIS SEMINAR WAS BROUGHT TO YOU BY:",
        "COPIOUS AMOUNTS OF CODING EVENINGS",
        "AND",
        "THE SHEER WILL TO LIVE"};

    for (const auto &line : credits)
    {
        gradPrint(line + "\n", 70);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    // ==== FINAL MESSAGE ====
    gradPrint("\n\nCONGRATULATIONS ON SURVIVING THE YEAR!\n");
    gradPrint("NOW GO CELEBRATE LIKE YOUR CODE FINALLY COMPILED.\n\n");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // ==== EXTRA: TERMINAL FLASHING (optional) ====
    for (int i = 0; i < 500; i++)
    {
        system("clear || cls");
        std::cout << getRandomColor() << "!!! YOU DID IT !!!" << COLOR_RESET << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}