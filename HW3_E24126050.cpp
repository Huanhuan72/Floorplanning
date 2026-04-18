#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <regex>
#include <iterator>
#include <queue>

using namespace std;

struct Module {
    std::string name;
    int width, height;
    int x, y; // 左下角座標
    int Sx,Sy;//slack
    Module() {} // 預設建構子
    Module(const std::string& name_, int w, int h)
        : name(name_), width(w), height(h), x(0), y(0), Sx(0), Sy(0) {}
};
class Graph {
    public:
        // 模組名稱 -> 模組資訊
        std::unordered_map<std::string, Module> modules;
    
        // 模組名稱 -> 鄰接邊
        std::unordered_map<std::string, std::vector<std::string>> adj_list;
    
        // 加入一個模組
        void addModule(const std::string& name, int width, int height) {
            modules[name] = Module{name, width, height};
            // 也要確保圖中有這個節點
            if (adj_list.find(name) == adj_list.end()) {
                adj_list[name] = {};
            }
        }
    
        // 加入一個邊：模組 a → 模組 b（表示 a 必須在 b 的左邊或下方）
        void addEdge(const std::string& from, const std::string& to) {
            if (adj_list.find(from) == adj_list.end()) adj_list[from] = {};
            if (adj_list.find(to) == adj_list.end()) adj_list[to] = {};
            adj_list[from].push_back(to);
        }
    
        // 取得模組資訊
        Module getModule(const std::string& name) {
            return modules.at(name);
        }
    
        // 取得相鄰模組
        std::vector<std::string> getNeighbors(const std::string& name) {
            return adj_list.at(name);
        }
    };
    bool safeGetline(std::istream& is, std::string& line) {
        if (!std::getline(is, line))
            return false;
    
        // 清除尾端的 \r 或 \n 或空白
        line.erase(line.find_last_not_of("\r\n ") + 1);
        return true;
    }
void readInputFile(const string& filename,unordered_map<std::string, Module>& modules,Graph& hcg, Graph& vcg) {
   
    ifstream inputfile(filename + ".txt");
    
    int moduleCount = 0, hcgCount = 0, vcgCount = 0;
    char ch1,ch2,ch3;
    string inputLine, token,module_dimension;
    string from, to;
    safeGetline(inputfile, inputLine);
    istringstream(inputLine) >> token>>token >> token >>token>> moduleCount;
    //outputTxt << "token : " << token << "\n"<< endl;
    //cout << "moduleCount : " << moduleCount << "\n"<< endl;
    safeGetline(inputfile, inputLine);
    //std::cout << "123Read line: [" << inputLine << "]" << std::endl;
    safeGetline(inputfile, inputLine);
    for(int i=0;i<moduleCount;i++)
    {
        Module module;
        safeGetline(inputfile, inputLine);
        //std::cout << "Read line: [" << inputLine << "]" << std::endl;
        istringstream(inputLine) >> token >> module.name >> token>>module_dimension;
        stringstream ss(module_dimension);
        ss >> ch1 >> module.width >> ch2 >> module.height >> ch3;

        hcg.addModule(module.name, module.width, module.height);
        vcg.addModule(module.name, module.width, module.height);
        modules[module.name] = module;

        //outputTxt << "W、H:"<<module.name<< "、"<<module.width <<"、" <<module.height << "\n"<< endl;
    }
    /*cout << "=== Modules ===" << std::endl;
    for (const pair<string, Module>& pair : modules) {
    const Module& m = pair.second;
    cout << "Module: " << m.name
              << ", Width: " << m.width
              << ", Height: " << m.height
              << ", X: " << m.x
              << ", Y: " << m.y << endl;
    }*/
    safeGetline(inputfile, inputLine);
    safeGetline(inputfile, inputLine);
    istringstream(inputLine) >> token>>token >> token >>token>>token>>token>> hcgCount;
    //cout << "HCGCount : " << hcgCount << "\n"<< endl;
    safeGetline(inputfile, inputLine);
    //std::cout << "Read line: [" << inputLine << "]" << std::endl;
    stringstream hcgStream(inputLine);
    string edge;
    while (getline(hcgStream, edge, ',')) {
        stringstream edgeStream(edge);
        string dummy;
        edgeStream >> from >> dummy >> to;
        hcg.addEdge(from, to);
        //outputTxt << "HCG: " << from << " to " << to << "\n" << endl;
    }
    
    /*cout << "=== Adjacency List ===\nHCG:" << endl;
    for (const auto& pair : hcg.adj_list) {
        cout << pair.first << " -> ";
        for (const string& neighbor : pair.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }*/
    
    safeGetline(inputfile, inputLine);
    safeGetline(inputfile, inputLine);
    istringstream(inputLine) >> token>>token >> token >>token>>token>>token>> vcgCount;
    //cout << "VCGCount : " << vcgCount << "\n"<< endl;
    safeGetline(inputfile, inputLine);
    //std::cout << "Read line: [" << inputLine << "]" << std::endl;
    stringstream vcgStream(inputLine);
   
    while (getline(vcgStream, edge, ',')) {
        stringstream edgeStream(edge);
        string dummy;
        edgeStream >> from >> dummy >> to;
        vcg.addEdge(from, to);
        //outputTxt << "VCG: " << from << " to " << to << "\n" << endl;
    }
    /*
    cout << "VCG:" << endl;
    for (const auto& pair : vcg.adj_list) {
        cout << pair.first << " -> ";
        for (const string& neighbor : pair.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }*/
    inputfile.close();
}

vector<string> topologicalSort(const unordered_map<string, vector<string>>& adjList) {
    unordered_map<string, int> inDegree;
    for (const auto& pair : adjList) {
        const string& u = pair.first;
        if (inDegree.find(u) == inDegree.end()) inDegree[u] = 0;
        for (const string& v : pair.second) {
            inDegree[v]++;
        }
    }

    queue<string> q;
    for (const auto& pair : inDegree) {
        if (pair.second == 0) q.push(pair.first);
    }

    vector<string> result;
    while (!q.empty()) {
        string u = q.front(); q.pop();
        result.push_back(u);
        for (const string& v : adjList.at(u)) {
            inDegree[v]--;
            if (inDegree[v] == 0) q.push(v);
        }
    }
    return result;
}
void computeCoordinates(unordered_map<string, Module>& modules,const unordered_map<string, vector<string>>& hcg,const unordered_map<string, vector<string>>& vcg)
{
    vector<string> topoOrder = topologicalSort(hcg);  // 水平為主

    for (const string& name : topoOrder) {
        Module& m = modules[name];

        // 計算 x（HCG）
        int maxRight = 0;
        for (const auto& pair : hcg) {
            const string& u = pair.first;
            for (const string& v : pair.second) {
                if (v == name) {
                    int right = modules[u].x + modules[u].width;
                    if (right > maxRight) maxRight = right;
                }
            }
        }
        m.x = maxRight;
    }
    topoOrder = topologicalSort(vcg);  // 水平為主
    for (const string& name : topoOrder) {
        Module& m = modules[name];
        // 計算 y（VCG）
        int maxTop = 0;
        for (const auto& pair : vcg) {
            const string& u = pair.first;
            for (const string& v : pair.second) {
                if (v == name) {
                    int top = modules[u].y + modules[u].height;
                    if (top > maxTop) maxTop = top;
                }
            }
        }
        m.y = maxTop;
    }
}
pair<int, int> computeFloorplanSize(
    const unordered_map<string, Module>& modules,
    const unordered_map<string, vector<string>>& hcg,
    const unordered_map<string, vector<string>>& vcg
) {
    int maxX = 0;
    int maxY = 0;

    for(const auto& pair : modules)
    {
        const Module& m = pair.second;
        int right = m.x + m.width;
        if (right > maxX) maxX = right;

        int top = m.y + m.height;
        if (top > maxY) maxY = top;
    }
    return make_pair(maxX, maxY);  // return (W, H)
}
void computeCoordinatesOpposite(
    unordered_map<string, Module>& modules,
    const unordered_map<string, vector<string>>& hcg,
    const unordered_map<string, vector<string>>& vcg,
    int floorplanWidth,
    int floorplanHeight,
    unordered_map<string, Module>& modulesOpposite // 儲存第二次擺法
) {
    // 初始化：複製一次 module 結構
    for (const auto& pair : modules) {
        modulesOpposite[pair.first] = pair.second;
    }

    // 用拓撲排序反過來排（從後面往前處理）
    vector<string> order = topologicalSort(hcg);
    reverse(order.begin(), order.end());  // 反向處理

    for (const string& name : order) {
        Module& m = modulesOpposite[name];

        // 處理 x
        int x_min = floorplanWidth - m.width;
        bool hasSuccessor = false;
        for (const auto& pair : hcg) {
            const string& u = pair.first;
            for (const string& v : pair.second) {
                if (u == name) {
                    hasSuccessor = true;
                    int left = modulesOpposite[v].x - m.width;
                    if (left < x_min) x_min = left;
                }
            }
        }
        m.x = x_min;

        // 處理 y
        int y_min = floorplanHeight - m.height;
        hasSuccessor = false;
        for (const auto& pair : vcg) {
            const string& u = pair.first;
            for (const string& v : pair.second) {
                if (u == name) {
                    hasSuccessor = true;
                    int below = modulesOpposite[v].y - m.height;
                    if (below < y_min) y_min = below;
                }
            }
        }
        m.y = y_min;
    }
}
void computeSlackAndFindCriticalEdges(
    unordered_map<string, Module>& modules,           // 公式1結果
    const unordered_map<string, Module>& modulesOpposite,   // 公式3結果
    const unordered_map<string, vector<string>>& hcg,
    const unordered_map<string, vector<string>>& vcg,
    vector<pair<string, string>>& criticalEdgesH,           // 水平 critical edges
    vector<pair<string, string>>& criticalEdgesV,           // 垂直 critical edges
    int& HCE,int& VCE
) {
    // 處理 HCG 的 slack
    for (const pair<string, Module>& pair : modules) {
        const string& name = pair.first;
        int x1 = modules.at(name).x;
        int x2 = modulesOpposite.at(name).x;
        int slack = x2 - x1 ;
        modules[name].Sx = slack;
    }
    for (const auto& pair : hcg) {
        const string& from = pair.first;
        for (const string& to : pair.second) {
            int S1 = modules.at(from).Sx;
            int S2 = modules.at(to).Sx;

            if (S1 == 0 && S2== 0) {
                criticalEdgesH.push_back(make_pair(from, to));
                HCE++;
            }
        }
    }
    // 處理 VCG 的 slack
    for (const pair<string, Module>& pair : modules) {
        const string& name = pair.first;
        int y1 = modules.at(name).y;
        int y2 = modulesOpposite.at(name).y;
        int slack = y2 - y1 ;
        modules[name].Sy = slack;
    }
    for (const auto& pair : vcg) {
        const string& from = pair.first;
        for (const string& to : pair.second) {
            int S1 = modules.at(from).Sy;
            int S2 = modules.at(to).Sy;

            if (S1 == 0 && S2== 0) {
                criticalEdgesV.push_back(make_pair(from, to));
                VCE++;
            }
        }
    }
}
void tryAllHorizontalCriticalEdges(
    const unordered_map<string, Module>& originalModules,
    const unordered_map<string, vector<string>>& originalHCG,
    const unordered_map<string, vector<string>>& originalVCG,
    const vector<pair<string, string>>& criticalEdgesH,
    int& minArea,
    int& bestWidth,
    int& bestHeight
) {
    for (const auto& edge : criticalEdgesH) {
        string from = edge.first;
        string to = edge.second;

        // 1. 拷貝 HCG 和 VCG
        unordered_map<string, vector<string>> hcg = originalHCG;
        unordered_map<string, vector<string>> vcg = originalVCG;

        // 2. 從 HCG 移除 edge from → to
        auto& hvec = hcg[from];
        hvec.erase(remove(hvec.begin(), hvec.end(), to), hvec.end());

        // 3. 如果 VCG 中沒有 from → to，加入
        auto& vvec = vcg[from];
        if (find(vvec.begin(), vvec.end(), to) == vvec.end()) {
            vvec.push_back(to);
        }

        // 4. 用新的 graph 重算座標與面積
        unordered_map<string, Module> modules = originalModules;
        computeCoordinates(modules, hcg, vcg);
        pair<int, int> wh = computeFloorplanSize(modules, hcg, vcg);
        int area = wh.first * wh.second;

        if (area < minArea) {
            minArea = area;
            bestWidth = wh.first;
            bestHeight = wh.second;
        }
    }
}


int main(int argc, char* argv[]) {
    // 1. 讀取輸入檔案（模組尺寸 + HCG/VCG） 
    if (argc < 2) {
        cerr << "Please add benchmark_name" << endl;
        return 1;
    }
   
    unordered_map<string, Module> modules;
    Graph hcg,vcg;  
    readInputFile(argv[1], modules, hcg, vcg);

     // 2. 計算第一次的座標（使用公式1）
    computeCoordinates(modules, hcg.adj_list, vcg.adj_list);
    /*
    cout << "\nModule coordinates (formula 1):\n";
    for (const auto& entry : modules) {
        const Module& m = entry.second;
        cout << m.name << ": (" << m.x << ", " << m.y << ")\n";
    }
    */
    // 3. 計算 floorplan 面積
    pair<int, int> wh = computeFloorplanSize(modules, hcg.adj_list, vcg.adj_list);
    int width = wh.first;
    int height = wh.second;
    int area = width * height;
/*
    cout << "\nFloorplan size:\n";
    cout << "Width = " << width << ", Height = " << height << endl;
    cout << "Area = " << area << endl;
    */
    // 4. 計算 slack（公式4）與 critical edges
    unordered_map<string, Module> modulesOpposite;
    computeCoordinatesOpposite(modules, hcg.adj_list, vcg.adj_list, width, height, modulesOpposite);
/*
    cout << "\nModule coordinates (formula 3 — right-top aligned):\n";
    for (const auto& entry : modulesOpposite) {
        const Module& m = entry.second;
        cout << m.name << ": (" << m.x << ", " << m.y << ")\n";

    }
  */  
    int HCE=0,VCE=0;
    vector<pair<string, string>> criticalH, criticalV;
    computeSlackAndFindCriticalEdges(modules, modulesOpposite,
                                    hcg.adj_list, vcg.adj_list,
                                    criticalH, criticalV,HCE,VCE);

    /*
    cout << "\n=== Critical Edges ===\nHorizontal:\n";
    for (const auto& p : criticalH) {
        cout << p.first << " to " << p.second << endl;
    }
    cout << "\nVertical:\n";
    for (const auto& p : criticalV) {
        cout << p.first << " to " << p.second << endl;
    }
        */
    // 5. 嘗試所有水平 critical edge 移除＋轉入 VCG 的情況，找最小面積
    int bestArea = INT32_MAX;
    int bestW = 0, bestH = 0;

    tryAllHorizontalCriticalEdges(modules, hcg.adj_list, vcg.adj_list,
                                criticalH, bestArea, bestW, bestH);
/*
    cout << "\n==== Optimized Floorplan ====\n";
    cout << "Min Width = " << bestW << ", Height = " << bestH << endl;
    cout << "Min Area = " << bestArea << endl;
*/
    // 6. 輸出結果到 .txt 檔

    string filename=argv[1];
    ofstream outputTxt(filename + "_E24126050.txt");

    sort(criticalH.begin(), criticalH.end(), [](const pair<string, string>& a, const pair<string, string>& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });
    sort(criticalV.begin(), criticalV.end(), [](const pair<string, string>& a, const pair<string, string>& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });
    outputTxt  << "number of horizontal critical edges " << HCE << endl;
    for (const auto& p : criticalH) {
        outputTxt << p.first << " to " << p.second << endl;
    }
    outputTxt << endl;
    outputTxt  << "number of vertical critical edges " << VCE << endl;
    for (const auto& p : criticalV) {
        outputTxt << p.first << " to " << p.second << endl;
    }
    outputTxt << endl;
    outputTxt  << "minimum floorplan area " << bestArea;
    outputTxt.close();
}