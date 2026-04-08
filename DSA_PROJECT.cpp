#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

struct Food {
    string name;
    int cost;    // Cost in cents
    int calories;

    Food(string n, double price, int cal)
        : name(n), cost(static_cast<int>(price * 100 + 0.5)), calories(cal) {}
};

pair<int, vector<int>> knapsackWithSolution(int targetCalories, vector<Food>& foods) {
    int n = foods.size();
    const int INF = 999999;
    vector<vector<int>> dp(targetCalories + 1, vector<int>(n + 1, INF));
    vector<vector<vector<int>>> selections(targetCalories + 1, vector<vector<int>>(n + 1));

    for (int i = 0; i <= n; i++) {
        dp[0][i] = 0;
    }

    for (int i = 1; i <= n; i++) {
        for (int cal = 0; cal <= targetCalories; cal++) {
            dp[cal][i] = dp[cal][i - 1];
            selections[cal][i] = selections[cal][i - 1];

            if (foods[i - 1].calories <= cal) {
                int newCost = foods[i - 1].cost + dp[cal - foods[i - 1].calories][i - 1];
                if (newCost < dp[cal][i]) {
                    dp[cal][i] = newCost;
                    selections[cal][i] = selections[cal - foods[i - 1].calories][i - 1];
                    selections[cal][i].push_back(i - 1);
                }
            }
        }
    }

    // If no exact solution, find the closest possible
    if (dp[targetCalories][n] >= INF) {
        for (int cal = targetCalories - 1; cal >= 0; cal--) {
            if (dp[cal][n] < INF) {
                return {cal, selections[cal][n]};
            }
        }
        return {0, {}}; // No solution at all
    }

    return {targetCalories, selections[targetCalories][n]};
}

void printSolution(int targetCalories, vector<Food>& foods) {
    auto [achievedCalories, selectedIndices] = knapsackWithSolution(targetCalories, foods);

    if (selectedIndices.empty()) {
        cout << "No possible solution with current food items." << endl;
        return;
    }

    cout << "\nOPTIMAL SOLUTION" << endl;
    if (achievedCalories == targetCalories) {
        cout << "Target: " << targetCalories << " calories (exact match)" << endl;
    } else {
        cout << "Target: " << targetCalories << " calories (closest match: " << achievedCalories << " calories)" << endl;
    }

    int totalCost = 0;
    int totalCal = 0;

    cout << "\nSelected foods:" << endl;
    for (int idx : selectedIndices) {
        cout << "• " << foods[idx].name
             << " - $" << fixed << setprecision(2) << (foods[idx].cost / 100.0)
             << " (" << foods[idx].calories << " calories)" << endl;
        totalCost += foods[idx].cost;
        totalCal += foods[idx].calories;
    }

    cout << "\nTotal: " << totalCal << " calories, $" << fixed << setprecision(2) << (totalCost / 100.0) << endl;
    cout << "Cost per calorie: $" << fixed << setprecision(2) << (totalCost / 100.0) / totalCal << endl;
}

int main() {
    cout << "SIMPLE KNAPSACK MEAL PLANNER" << endl;
    cout << "=============================" << endl;
    cout << "Goal: Find optimal meal plan for target calories" << endl;

    vector<Food> foods = {
        Food("Rice Bowl", 3.50, 300),
        Food("Chicken Breast", 5.00, 200),
        Food("Banana", 1.00, 100),
        Food("Bread Slice", 1.50, 80),
        Food("Apple", 0.75, 50),
        Food("Pasta Plate", 4.00, 250),
        Food("Yogurt Cup", 2.50, 120),
        Food("Boiled Egg", 2.00, 70),
        Food("Granola Bar", 1.25, 90),
        Food("Orange", 0.80, 60),
        Food("Fish Fillet", 8.00, 167),
        Food("Nuts Mix", 4.50, 345),
        Food("Carrot Sticks", 1.00, 98),
        Food("Cottage Cheese", 2.00, 145),
        Food("Dried Fruits", 3.00, 126),
        Food("Rice Cake", 0.50, 45),
        Food("1 Boiled Egg", 0.70, 20),
        Food("1 cucumber", 0.5, 10),
        Food("1 tomato", 0.5, 3)

    };

    cout << "\nAvailable foods:" << endl;
    cout << "No. | Food Item      | Price | Calories | Value (cal/$)" << endl;
    cout << "----|----------------|-------|----------|---------------" << endl;

    for (int i = 0; i < foods.size(); i++) {
        double value = foods[i].calories / (foods[i].cost / 100.0);
        cout << (i + 1) << ".  | " << foods[i].name;

        int padding = 15 - foods[i].name.length();
        for (int j = 0; j < padding; j++) cout << " ";

        cout << "| $" << fixed << setprecision(2) << (foods[i].cost / 100.0)
             << "  | " << foods[i].calories
             << "      | " << (int)value << endl;
    }

    vector<int> targets = {22, 200, 350, 500, 750};

    for (int target : targets) {
        cout << "\n" << string(50, '=') << endl;
        cout << "TEST: " << target << " calories target" << endl;
        printSolution(target, foods);
    }

    cout << "\n" << string(50, '=') << endl;
    cout << "Try your own target!" << endl;
    cout << "Enter target calories (0 to exit): ";

    int userTarget;
    while (cin >> userTarget && userTarget > 0) {
        printSolution(userTarget, foods);
        cout << "\nEnter another target (0 to exit): ";
    }

    cout << "Thanks for using the meal planner!" << endl;
    return 0;
}