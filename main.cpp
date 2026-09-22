#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <iomanip>

using namespace std;

// Structure representing a single node in a Decision Tree
struct DecisionNode {
    string name;
    int time;       // 1-10
    int effort;     // 1-10
    int skillGain;  // 1-10
    int goalMatch;  // 1-10
    int risk;       // 1-10
    int moodBoost;  // 1-10

    vector<shared_ptr<DecisionNode>> children;
     //constructors
    DecisionNode(string n, int t, int e, int s, int g, int r, int m = 5)
        : name(n), time(t), effort(e), skillGain(s), goalMatch(g), risk(r), moodBoost(m) {}

    double getScore(int energyWeight, int moodWeight) const {
        double benefit = (skillGain * 1.5) + (goalMatch * 1.8) + (moodBoost * moodWeight);
        double cost = (time * 0.8) + (effort * energyWeight * 0.2) + (risk * 1.2);
        return benefit - cost;
    }
};

// Structure representing a decision trajectory
struct DecisionPath {
    vector<string> pathNames;
    double cumulativeScore = 0.0;
    int totalRisk = 0;
    int totalTime = 0;

    bool operator<(const DecisionPath& other) const {
        return this->cumulativeScore < other.cumulativeScore;
    }
};

struct OptionItem {
    string key;
    string title;
};

// Global Database
unordered_map<string, shared_ptr<DecisionNode>> predefinedDB;
unordered_map<int, vector<OptionItem>> moodOptionsCatalog;

void initializeDatabase() {
    // =========================================================================
    // 1. PRODUCTIVE / FOCUSED ACTIVITIES
    // =========================================================================
    
    // Study / Revision
    auto study = make_shared<DecisionNode>("Study / Revision", 4, 6, 9, 9, 2, 4);
    study->children.push_back(make_shared<DecisionNode>("Revise Core Theory Notes", 2, 4, 7, 8, 1, 3));
    study->children.push_back(make_shared<DecisionNode>("Solve Practice Question Sets", 4, 7, 9, 9, 2, 5));
    predefinedDB["study"] = study;

    // Coding / DSA Practice
    auto dsa = make_shared<DecisionNode>("Coding / DSA Practice", 5, 8, 10, 10, 3, 5);
    dsa->children.push_back(make_shared<DecisionNode>("Solve LeetCode / HackerRank", 5, 8, 10, 10, 4, 6));
    dsa->children.push_back(make_shared<DecisionNode>("Debug Code & Review Data Structures", 3, 6, 8, 9, 2, 4));
    predefinedDB["dsa"] = dsa;

    // Assignment Work
    auto assignment = make_shared<DecisionNode>("Assignment Work", 4, 6, 7, 9, 2, 4);
    assignment->children.push_back(make_shared<DecisionNode>("Write Lab Reports & Documentation", 3, 5, 6, 9, 1, 3));
    assignment->children.push_back(make_shared<DecisionNode>("Complete Pending Coursework", 4, 7, 7, 9, 2, 5));
    predefinedDB["assignment"] = assignment;

    // Reading / Research
    auto research = make_shared<DecisionNode>("Reading / Research", 3, 4, 8, 8, 1, 6);
    research->children.push_back(make_shared<DecisionNode>("Read Technical Articles / Papers", 3, 5, 9, 8, 1, 6));
    research->children.push_back(make_shared<DecisionNode>("Explore New Tech Documentation", 2, 3, 7, 7, 1, 7));
    predefinedDB["research"] = research;

    // Project Work
    auto project = make_shared<DecisionNode>("Project Work", 6, 8, 9, 10, 4, 8);
    project->children.push_back(make_shared<DecisionNode>("Build Core Backend / Modules", 5, 9, 10, 10, 5, 8));
    project->children.push_back(make_shared<DecisionNode>("Design Modular UI / Architecture", 3, 5, 7, 8, 2, 7));
    predefinedDB["project"] = project;

    // Problem Solving / Puzzles
    auto puzzles = make_shared<DecisionNode>("Problem Solving / Puzzles", 3, 6, 8, 7, 2, 7);
    puzzles->children.push_back(make_shared<DecisionNode>("Solve Logic Puzzles & Brain Teasers", 2, 5, 7, 6, 1, 8));
    puzzles->children.push_back(make_shared<DecisionNode>("Competitive Programming Challenge", 4, 8, 9, 8, 3, 6));
    predefinedDB["puzzles"] = puzzles;

    // Organizing Tasks / Planning
    auto planning = make_shared<DecisionNode>("Organizing Tasks / Planning", 2, 2, 5, 9, 1, 7);
    planning->children.push_back(make_shared<DecisionNode>("Organize To-Do List & Weekly Goals", 2, 2, 5, 9, 1, 7));
    planning->children.push_back(make_shared<DecisionNode>("Clean Workspace & Digital Files", 2, 3, 4, 8, 1, 8));
    predefinedDB["planning"] = planning;

    // Resume / Career Work
    auto career = make_shared<DecisionNode>("Resume / Career Work", 3, 5, 8, 9, 2, 6);
    career->children.push_back(make_shared<DecisionNode>("Update Resume & LinkedIn Profile", 2, 4, 8, 9, 2, 6));
    career->children.push_back(make_shared<DecisionNode>("Apply to Internships / Jobs", 3, 6, 8, 10, 3, 5));
    predefinedDB["career"] = career;

    // Exam Preparation
    auto examPrep = make_shared<DecisionNode>("Exam Preparation", 5, 8, 9, 10, 3, 4);
    examPrep->children.push_back(make_shared<DecisionNode>("Solve Previous Year Papers", 4, 8, 9, 10, 3, 4));
    examPrep->children.push_back(make_shared<DecisionNode>("Create Quick Summary Formula Sheets", 3, 5, 8, 9, 1, 5));
    predefinedDB["examPrep"] = examPrep;

    // Learning a New Skill
    auto newSkill = make_shared<DecisionNode>("Learning a New Skill", 4, 6, 9, 8, 2, 8);
    newSkill->children.push_back(make_shared<DecisionNode>("Watch Tutorial / Take Online Course", 3, 5, 8, 8, 1, 7));
    newSkill->children.push_back(make_shared<DecisionNode>("Hands-on Skill Sandbox Testing", 4, 7, 9, 8, 2, 8));
    predefinedDB["newSkill"] = newSkill;

    // Working on a Personal Project
    auto personalProj = make_shared<DecisionNode>("Working on a Personal Project", 5, 7, 9, 9, 3, 9);
    personalProj->children.push_back(make_shared<DecisionNode>("Build Custom Automation Script", 3, 6, 9, 9, 2, 8));
    personalProj->children.push_back(make_shared<DecisionNode>("Design & Launch Personal Feature", 5, 8, 9, 9, 3, 9));
    predefinedDB["personalProj"] = personalProj;

    // Pomodoro / Deep Work Session
    auto pomodoro = make_shared<DecisionNode>("Pomodoro / Deep Work Session", 4, 7, 8, 9, 2, 6);
    pomodoro->children.push_back(make_shared<DecisionNode>("25-min Sprint: High Intensity Focus", 2, 6, 8, 9, 1, 6));
    pomodoro->children.push_back(make_shared<DecisionNode>("50-min Sprint: Deep Flow State", 4, 8, 9, 10, 2, 7));
    predefinedDB["pomodoro"] = pomodoro;

    // Add to Mood 1 Catalog
    moodOptionsCatalog[1] = {
        {"study", "Study / Revision"},
        {"dsa", "Coding / DSA Practice"},
        {"assignment", "Assignment Work"},
        {"research", "Reading / Research"},
        {"project", "Project Work"},
        {"puzzles", "Problem Solving / Puzzles"},
        {"planning", "Organizing Tasks / Planning"},
        {"career", "Resume / Career Work"},
        {"examPrep", "Exam Preparation"},
        {"newSkill", "Learning a New Skill"},
        {"personalProj", "Working on a Personal Project"},
        {"pomodoro", "Pomodoro / Deep Work Session"}
    };

    // =========================================================================
    // 2. EXHAUSTED / STRESSED ACTIVITIES
    // =========================================================================

    // Power Nap
    auto powerNap = make_shared<DecisionNode>("Power Nap", 2, 1, 1, 3, 1, 8);
    powerNap->children.push_back(make_shared<DecisionNode>("20-min Refreshing Power Nap", 1, 1, 1, 3, 1, 8));
    predefinedDB["powerNap"] = powerNap;

    // Meditation
    auto meditation = make_shared<DecisionNode>("Meditation", 2, 1, 2, 5, 1, 9);
    meditation->children.push_back(make_shared<DecisionNode>("10-min Guided Mindfulness Session", 1, 1, 2, 5, 1, 9));
    predefinedDB["meditation"] = meditation;

    // Breathing Exercise
    auto breathing = make_shared<DecisionNode>("Breathing Exercise", 1, 1, 1, 4, 1, 8);
    breathing->children.push_back(make_shared<DecisionNode>("Box Breathing / Calm Routine", 1, 1, 1, 4, 1, 8));
    predefinedDB["breathing"] = breathing;

    // Short Walk
    auto shortWalk = make_shared<DecisionNode>("Short Walk", 2, 2, 2, 5, 1, 8);
    shortWalk->children.push_back(make_shared<DecisionNode>("Stroll in Nature / Nearby Park", 2, 2, 2, 5, 1, 9));
    predefinedDB["shortWalk"] = shortWalk;

    // Listen to Music
    auto music = make_shared<DecisionNode>("Listen to Music", 2, 1, 1, 3, 1, 9);
    music->children.push_back(make_shared<DecisionNode>("Relaxing Instrumental / Lo-Fi Playlist", 2, 1, 1, 3, 1, 9));
    predefinedDB["music"] = music;

    // Sit / Relax Quietly
    auto sitQuietly = make_shared<DecisionNode>("Sit / Relax Quietly", 2, 1, 1, 3, 1, 8);
    sitQuietly->children.push_back(make_shared<DecisionNode>("Unwind on Couch with Zero Screen Time", 2, 1, 1, 3, 1, 8));
    predefinedDB["sitQuietly"] = sitQuietly;

    // Digital Detox
    auto detox = make_shared<DecisionNode>("Digital Detox", 3, 2, 3, 6, 1, 9);
    detox->children.push_back(make_shared<DecisionNode>("Turn Off Phone & Screens for 1 Hour", 3, 2, 3, 6, 1, 9));
    predefinedDB["detox"] = detox;

    // Take a Break
    auto breakTime = make_shared<DecisionNode>("Take a Break", 1, 1, 1, 4, 1, 7);
    breakTime->children.push_back(make_shared<DecisionNode>("Quick 15-min Screen Break", 1, 1, 1, 4, 1, 7));
    predefinedDB["breakTime"] = breakTime;

    // Journaling
    auto journaling = make_shared<DecisionNode>("Journaling", 2, 2, 3, 6, 1, 8);
    journaling->children.push_back(make_shared<DecisionNode>("Write Down Thoughts & Mood Logs", 2, 2, 3, 6, 1, 8));
    predefinedDB["journaling"] = journaling;

    // Stretching
    auto stretching = make_shared<DecisionNode>("Stretching", 2, 2, 3, 6, 1, 8);
    stretching->children.push_back(make_shared<DecisionNode>("Light Full-Body Stretch Routine", 2, 2, 3, 6, 1, 8));
    predefinedDB["stretching"] = stretching;

    // Hydration / Refreshment Break
    auto hydration = make_shared<DecisionNode>("Hydration Break", 1, 1, 1, 5, 1, 7);
    hydration->children.push_back(make_shared<DecisionNode>("Drink Water & Grab Healthy Snack", 1, 1, 1, 5, 1, 7));
    predefinedDB["hydration"] = hydration;

    // Go Outside / Get Fresh Air
    auto freshAir = make_shared<DecisionNode>("Go Outside / Fresh Air", 2, 2, 2, 5, 1, 9);
    freshAir->children.push_back(make_shared<DecisionNode>("Sit Balcony / Sunlight Exposure", 2, 1, 2, 5, 1, 9));
    predefinedDB["freshAir"] = freshAir;

    // Talk to a Friend / Family
    auto talkFriend = make_shared<DecisionNode>("Talk to Friend / Family", 3, 2, 2, 7, 1, 9);
    talkFriend->children.push_back(make_shared<DecisionNode>("Quick Phone Call / Venting Session", 2, 2, 2, 7, 1, 9));
    predefinedDB["talkFriend"] = talkFriend;

    // Rest / Sleep
    auto restSleep = make_shared<DecisionNode>("Rest / Sleep", 7, 1, 1, 3, 1, 10);
    restSleep->children.push_back(make_shared<DecisionNode>("Full Sleep Recharge Cycle", 7, 1, 1, 3, 1, 10));
    predefinedDB["restSleep"] = restSleep;

    // Add to Mood 2 Catalog
    moodOptionsCatalog[2] = {
        {"powerNap", "Power Nap"},
        {"meditation", "Meditation"},
        {"breathing", "Breathing Exercise"},
        {"shortWalk", "Short Walk"},
        {"music", "Listen to Music"},
        {"sitQuietly", "Sit / Relax Quietly"},
        {"detox", "Digital Detox"},
        {"breakTime", "Take a Break"},
        {"journaling", "Journaling"},
        {"stretching", "Stretching"},
        {"hydration", "Hydration / Refreshment Break"},
        {"freshAir", "Go Outside / Get Fresh Air"},
        {"talkFriend", "Talk to a Friend / Family"},
        {"restSleep", "Rest / Sleep"}
    };

    // =========================================================================
    // 3. ACTIVE / HIGH ENERGY ACTIVITIES
    // =========================================================================

    // Running / Jogging
    auto running = make_shared<DecisionNode>("Running / Jogging", 3, 8, 5, 7, 2, 9);
    running->children.push_back(make_shared<DecisionNode>("3-Mile Outdoor Sprint / Run", 3, 8, 5, 7, 2, 9));
    predefinedDB["running"] = running;

    // Cycling
    auto cycling = make_shared<DecisionNode>("Cycling", 4, 7, 5, 7, 3, 9);
    cycling->children.push_back(make_shared<DecisionNode>("Scenic Neighborhood Cycling Ride", 4, 7, 5, 7, 3, 9));
    predefinedDB["cycling"] = cycling;

    // Workout
    auto workout = make_shared<DecisionNode>("Workout / Gym", 4, 8, 6, 8, 2, 9);
    workout->children.push_back(make_shared<DecisionNode>("Heavy Weightlifting Session", 5, 9, 7, 8, 3, 9));
    workout->children.push_back(make_shared<DecisionNode>("Bodyweight Calisthenics Routine", 3, 7, 6, 8, 1, 8));
    predefinedDB["workout"] = workout;

    // Sports
    auto sports = make_shared<DecisionNode>("Sports", 5, 8, 6, 7, 3, 10);
    sports->children.push_back(make_shared<DecisionNode>("Play Badminton / Basketball Match", 5, 8, 6, 7, 3, 10));
    predefinedDB["sports"] = sports;

    // Martial Arts
    auto martialArts = make_shared<DecisionNode>("Martial Arts", 4, 9, 7, 7, 4, 9);
    martialArts->children.push_back(make_shared<DecisionNode>("Kickboxing / Karate Drills", 4, 9, 7, 7, 4, 9));
    predefinedDB["martialArts"] = martialArts;

    // Dancing
    auto dance = make_shared<DecisionNode>("Dancing", 3, 7, 5, 6, 2, 10);
    dance->children.push_back(make_shared<DecisionNode>("Learn New Choreography", 4, 8, 7, 6, 2, 10));
    dance->children.push_back(make_shared<DecisionNode>("Freestyle High Energy Jam", 2, 6, 4, 5, 1, 10));
    predefinedDB["dance"] = dance;

    // Long Walk
    auto longWalk = make_shared<DecisionNode>("Long Walk", 4, 4, 3, 6, 1, 9);
    longWalk->children.push_back(make_shared<DecisionNode>("Explore New Walking Trail", 4, 4, 3, 6, 1, 9));
    predefinedDB["longWalk"] = longWalk;

    // Swimming
    auto swimming = make_shared<DecisionNode>("Swimming", 4, 8, 6, 7, 2, 10);
    swimming->children.push_back(make_shared<DecisionNode>("Lap Swimming Session", 4, 8, 6, 7, 2, 10));
    predefinedDB["swimming"] = swimming;

    // Outdoor Activity
    auto outdoorAct = make_shared<DecisionNode>("Outdoor Activity", 5, 6, 4, 6, 3, 9);
    outdoorAct->children.push_back(make_shared<DecisionNode>("Hiking / Nature Trail Exploration", 5, 6, 4, 6, 3, 9));
    predefinedDB["outdoorAct"] = outdoorAct;

    // Cleaning / Organizing Room
    auto cleaning = make_shared<DecisionNode>("Cleaning / Organizing Room", 3, 5, 3, 7, 1, 8);
    cleaning->children.push_back(make_shared<DecisionNode>("Deep Clean Room & Desk Setup", 3, 5, 3, 7, 1, 8));
    predefinedDB["cleaning"] = cleaning;

    // Active Games
    auto activeGames = make_shared<DecisionNode>("Active Games", 3, 6, 3, 5, 2, 10);
    activeGames->children.push_back(make_shared<DecisionNode>("Motion / VR Fitness Gaming", 3, 6, 3, 5, 2, 10));
    predefinedDB["activeGames"] = activeGames;

    // Quick Fitness Challenge
    auto fitChallenge = make_shared<DecisionNode>("Quick Fitness Challenge", 2, 9, 5, 7, 2, 8);
    fitChallenge->children.push_back(make_shared<DecisionNode>("100-Pushups / Plank Burnout Challenge", 2, 9, 5, 7, 2, 8));
    predefinedDB["fitChallenge"] = fitChallenge;

    // Group Activity
    auto groupActivity = make_shared<DecisionNode>("Group Activity", 4, 6, 4, 7, 2, 9);
    groupActivity->children.push_back(make_shared<DecisionNode>("Group Workout / Outing", 4, 6, 4, 7, 2, 9));
    predefinedDB["groupActivity"] = groupActivity;

    // Goal-Oriented Challenge
    auto goalChallenge = make_shared<DecisionNode>("Goal-Oriented Challenge", 4, 8, 8, 9, 3, 9);
    goalChallenge->children.push_back(make_shared<DecisionNode>("Complete High-Energy Goal Sprint", 4, 8, 8, 9, 3, 9));
    predefinedDB["goalChallenge"] = goalChallenge;

    // Add to Mood 3 Catalog
    moodOptionsCatalog[3] = {
        {"running", "Running / Jogging"},
        {"cycling", "Cycling"},
        {"workout", "Workout"},
        {"sports", "Sports"},
        {"martialArts", "Martial Arts"},
        {"dance", "Dancing"},
        {"longWalk", "Long Walk"},
        {"swimming", "Swimming"},
        {"outdoorAct", "Outdoor Activity"},
        {"cleaning", "Cleaning / Organizing Room"},
        {"activeGames", "Active Games"},
        {"fitChallenge", "Quick Fitness Challenge"},
        {"groupActivity", "Group Activity"},
        {"goalChallenge", "Goal-Oriented Challenge"}
    };

    // =========================================================================
    // 4. RELAXED / CHILL MODE ACTIVITIES
    // =========================================================================

    // Listening to Music
    auto chillMusic = make_shared<DecisionNode>("Listening to Music", 2, 1, 1, 3, 1, 9);
    chillMusic->children.push_back(make_shared<DecisionNode>("Listen to Favorite Albums", 2, 1, 1, 3, 1, 9));
    predefinedDB["chillMusic"] = chillMusic;

    // Watching a Movie
    auto movie = make_shared<DecisionNode>("Watching a Movie", 5, 1, 2, 3, 1, 9);
    movie->children.push_back(make_shared<DecisionNode>("Watch Favorite Comfort Movie", 5, 1, 2, 3, 1, 9));
    predefinedDB["movie"] = movie;

    // Watching a Series
    auto series = make_shared<DecisionNode>("Watching a Series", 4, 1, 2, 3, 1, 8);
    series->children.push_back(make_shared<DecisionNode>("Binge 2 Episodes of TV Series", 4, 1, 2, 3, 1, 8));
    predefinedDB["series"] = series;

    // Casual Reading
    auto casualReading = make_shared<DecisionNode>("Casual Reading", 3, 2, 5, 5, 1, 8);
    casualReading->children.push_back(make_shared<DecisionNode>("Read Fiction Book / Manga", 3, 2, 5, 5, 1, 8));
    predefinedDB["casualReading"] = casualReading;

    // Gaming
    auto gaming = make_shared<DecisionNode>("Gaming", 4, 3, 3, 4, 1, 9);
    gaming->children.push_back(make_shared<DecisionNode>("Play Casual Video Game Session", 4, 3, 3, 4, 1, 9));
    predefinedDB["gaming"] = gaming;

    // Drawing / Sketching
    auto drawing = make_shared<DecisionNode>("Drawing / Sketching", 3, 3, 6, 5, 1, 9);
    drawing->children.push_back(make_shared<DecisionNode>("Freestyle Art & Doodling", 3, 3, 6, 5, 1, 9));
    predefinedDB["drawing"] = drawing;

    // Photography
    auto photography = make_shared<DecisionNode>("Photography", 3, 3, 5, 5, 1, 8);
    photography->children.push_back(make_shared<DecisionNode>("Take Aesthetic Photos Nearby", 3, 3, 5, 5, 1, 8));
    predefinedDB["photography"] = photography;

    // Gardening
    auto gardening = make_shared<DecisionNode>("Gardening", 3, 3, 3, 5, 1, 8);
    gardening->children.push_back(make_shared<DecisionNode>("Water & Tend to Houseplants", 3, 3, 3, 5, 1, 8));
    predefinedDB["gardening"] = gardening;

    // Having a Relaxing Drink
    auto relaxingDrink = make_shared<DecisionNode>("Having a Relaxing Drink", 1, 1, 1, 4, 1, 8);
    relaxingDrink->children.push_back(make_shared<DecisionNode>("Sip Hot Tea / Coffee Quietly", 1, 1, 1, 4, 1, 8));
    predefinedDB["relaxingDrink"] = relaxingDrink;

    // Casual Puzzles
    auto casualPuzzles = make_shared<DecisionNode>("Casual Puzzles", 2, 3, 4, 4, 1, 8);
    casualPuzzles->children.push_back(make_shared<DecisionNode>("Solve Wordle / Crossword Puzzle", 2, 3, 4, 4, 1, 8));
    predefinedDB["casualPuzzles"] = casualPuzzles;

    // Light Meditation
    auto lightMeditation = make_shared<DecisionNode>("Light Meditation", 2, 1, 2, 5, 1, 8);
    lightMeditation->children.push_back(make_shared<DecisionNode>("Soft Zen Breathing & Mind Unwind", 2, 1, 2, 5, 1, 8));
    predefinedDB["lightMeditation"] = lightMeditation;

    // Watching Sunset / Nature
    auto sunset = make_shared<DecisionNode>("Watching Sunset / Nature", 2, 1, 1, 4, 1, 10);
    sunset->children.push_back(make_shared<DecisionNode>("Watch Golden Hour Sunset Outside", 2, 1, 1, 4, 1, 10));
    predefinedDB["sunset"] = sunset;

    // Casual Chat with Friends
    auto casualChat = make_shared<DecisionNode>("Casual Chat with Friends", 3, 2, 2, 6, 1, 9);
    casualChat->children.push_back(make_shared<DecisionNode>("Hangout Chat / Voice Call", 3, 2, 2, 6, 1, 9));
    predefinedDB["casualChat"] = casualChat;

    // Podcast / Audiobook
    auto podcast = make_shared<DecisionNode>("Podcast / Audiobook", 3, 1, 5, 5, 1, 8);
    podcast->children.push_back(make_shared<DecisionNode>("Listen to Interesting Podcast", 3, 1, 5, 5, 1, 8));
    predefinedDB["podcast"] = podcast;

    // Chill Journaling
    auto chillJournaling = make_shared<DecisionNode>("Journaling", 2, 2, 3, 5, 1, 8);
    chillJournaling->children.push_back(make_shared<DecisionNode>("Light Daily Reflections Write-up", 2, 2, 3, 5, 1, 8));
    predefinedDB["chillJournaling"] = chillJournaling;

    // Chill Resting
    auto chillRest = make_shared<DecisionNode>("Resting", 3, 1, 1, 3, 1, 9);
    chillRest->children.push_back(make_shared<DecisionNode>("Lie Down & Relax Comfortably", 3, 1, 1, 3, 1, 9));
    predefinedDB["chillRest"] = chillRest;

    // Add to Mood 4 Catalog
    moodOptionsCatalog[4] = {
        {"chillMusic", "Listening to Music"},
        {"movie", "Watching a Movie"},
        {"series", "Watching a Series"},
        {"casualReading", "Casual Reading"},
        {"gaming", "Gaming"},
        {"drawing", "Drawing / Sketching"},
        {"photography", "Photography"},
        {"gardening", "Gardening"},
        {"relaxingDrink", "Having a Relaxing Drink"},
        {"casualPuzzles", "Casual Puzzles"},
        {"lightMeditation", "Light Meditation"},
        {"sunset", "Watching Sunset / Nature"},
        {"casualChat", "Casual Chat with Friends"},
        {"podcast", "Podcast / Audiobook"},
        {"chillJournaling", "Journaling"},
        {"chillRest", "Resting"}
    };
}

// Backtracking algorithm
void evaluatePathsBacktracking(const shared_ptr<DecisionNode>& currentNode,
                               vector<string>& currentPath,
                               double currentScore,
                               int currentRisk,
                               int currentTime,
                               int maxRisk,
                               int maxTime,
                               int energyWeight,
                               int moodWeight,
                               priority_queue<DecisionPath>& heap) {
    if (!currentNode) return;

    currentPath.push_back(currentNode->name);
    double totalScore = currentScore + currentNode->getScore(energyWeight, moodWeight);
    int totalRisk = currentRisk + currentNode->risk;
    int totalTime = currentTime + currentNode->time;

    if (totalRisk > maxRisk || totalTime > maxTime) {
        currentPath.pop_back();
        return;
    }

    if (currentNode->children.empty()) {
        DecisionPath dp;
        dp.pathNames = currentPath;
        dp.cumulativeScore = totalScore;
        dp.totalRisk = totalRisk;
        dp.totalTime = totalTime;
        heap.push(dp);
    } else {
        for (const auto& child : currentNode->children) {
            evaluatePathsBacktracking(child, currentPath, totalScore, totalRisk, totalTime, 
                                       maxRisk, maxTime, energyWeight, moodWeight, heap);
        }
    }

    currentPath.pop_back();
}

int main() {
    std::setvbuf(stdout, NULL, _IONBF, 0);
    initializeDatabase();

    cout << "====================================================\n";
    cout << "                     LIFE.EXE                       \n";
    cout << "          Smart Mood-Based Decision Engine          \n";
    cout << "====================================================\n\n";

    string situation;
    cout << "DO YOU WANT TO UTILIZE YOUR TIME PROPERLY?: ";
    getline(cin, situation);

    // Mood Selection
    int moodChoice;
    cout << "\nHow are you feeling right now? ENTER YOUR CURRENT MOOD!! \n";
    cout << " 1. Productive / Focused\n";
    cout << " 2. Exhausted / Stressed\n";
    cout << " 3. Active / High Energy\n";
    cout << " 4. Relaxed / Chill Mode\n";
    cout << "Select Mood Mode (1-4): ";
    cin >> moodChoice;

    if (moodChoice < 1 || moodChoice > 4) moodChoice = 1;

    int energyWeight = 1, moodWeight = 1;
    if (moodChoice == 1) { energyWeight = 1; moodWeight = 1; }
    else if (moodChoice == 2) { energyWeight = 3; moodWeight = 3; }
    else if (moodChoice == 3) { energyWeight = 1; moodWeight = 4; }
    else { energyWeight = 2; moodWeight = 2; }

    // Retrieve filtered catalog for selected mood
    vector<OptionItem> currentOptions = moodOptionsCatalog[moodChoice];

    cout << "\n====================================================\n";
    cout << "        SUGGESTED OPTIONS FOR YOUR MOOD             \n";
    cout << "====================================================\n";
    for (size_t i = 0; i < currentOptions.size(); i++) {
        cout << setw(2) << i + 1 << ". " << currentOptions[i].title << "\n";
    }

    int choiceCount;
    cout << "\nHow many options do you want to consider? ";
    cin >> choiceCount;

    vector<string> selectedKeys;
    cout << "Enter option numbers (e.g., '1 2 3'): ";
    for (int i = 0; i < choiceCount; i++) {
        int idx;
        cin >> idx;
        if (idx >= 1 && idx <= (int)currentOptions.size()) {
            selectedKeys.push_back(currentOptions[idx - 1].key);
        }
    }

    priority_queue<DecisionPath> pathHeap;
    int maxRisk = 20;
    int maxTime = 20;

    for (const string& key : selectedKeys) {
        if (predefinedDB.count(key)) {
            vector<string> path;
            evaluatePathsBacktracking(predefinedDB[key], path, 0.0, 0, 0, 
                                       maxRisk, maxTime, energyWeight, moodWeight, pathHeap);
        }
    }

    cout << "\n====================================================\n";
    cout << "              RECOMMENDED PREDICTIONS               \n";
    cout << "====================================================\n";

    if (pathHeap.empty()) {
        cout << "\nNo valid option found for the selected inputs.\n";
        return 0;
    }

    // Top Choice Display (Clean without numeric ratings)
    DecisionPath best = pathHeap.top();
    cout << "\n*** BEST RECOMMENDED CHOICE ***\n";
    cout << "-> ";
    for (size_t i = 0; i < best.pathNames.size(); i++) {
        cout << best.pathNames[i] << (i + 1 < best.pathNames.size() ? " -> " : "");
    }
    cout << "\n";

    // Clean Ranked List
    cout << "\n----------------------------------------------------\n";
    cout << "All Ranked Choices (Best to Worst):\n";
    cout << "----------------------------------------------------\n";

    int rank = 1;
    while (!pathHeap.empty()) {
        DecisionPath p = pathHeap.top();
        pathHeap.pop();

        cout << rank++ << ". ";
        for (size_t i = 0; i < p.pathNames.size(); i++) {
            cout << p.pathNames[i] << (i + 1 < p.pathNames.size() ? " -> " : "");
        }
        cout << "\n";
    }

    return 0;
}