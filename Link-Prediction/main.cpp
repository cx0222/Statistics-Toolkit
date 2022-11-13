#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include "methods.h"
#include "edge.h"

class network {
private:
    int vertex_count = 0;
    std::vector<std::vector<int>> edges;
    std::vector<struct edge> basic_index;
public:
    network() = default;

    explicit network(int vertex_count) : vertex_count(vertex_count) {
        edges.resize(vertex_count);
    }

    virtual ~network() = default;

    void initialize(const std::string &);

    void predict(const std::vector<std::string> &, int);

private:
    void read_csv(const std::string &);

    void get_basic_index(int, int);
};

int main() {
    auto net_science = new network(379);
    net_science->initialize("ca-netscience.txt");
    std::vector<std::string> methods = {"CN", "salton", "jaccard", "sorenson", "HPI", "HDI", "LHN", "PA", "AA", "RA"};
    net_science->predict(methods, 25);
    return 0;
}

void network::read_csv(const std::string &filename) {
    // 从 CSV 文件中读取数据
    std::ifstream ifstream;
    ifstream.open(filename, std::ios::in);
    if (!ifstream.is_open()) {
        printf("[错误] 未打开文件！");
        return;
    }
    std::string buffer;
    while (getline(ifstream, buffer)) {
        auto loc = buffer.find(' ');
        int start = std::stoi(buffer.substr(0, loc));
        int end = std::stoi(buffer.substr(loc + 1));
        edges[start - 1].emplace_back(end - 1);
        edges[end - 1].emplace_back(start - 1);
    }
    ifstream.close();
}

void network::get_basic_index(int i, int j) {
    // 计算出交集与并集的元素个数
    int intersection_size = 0, union_size = 0;
    std::vector<int> &edges1 = edges[i];
    std::vector<int> &edges2 = edges[j];
    auto ptr_1 = edges1.begin(), ptr_2 = edges2.begin();
    auto end_1 = edges1.end(), end_2 = edges2.end();
    while (ptr_1 < end_1 && ptr_2 < end_2) {
        if (*ptr_1 == *ptr_2) {
            ++intersection_size;
            ++ptr_1;
            ++ptr_2;
        } else if (*ptr_1 < *ptr_2) {
            ++ptr_1;
        } else {
            ++ptr_2;
        }
    }
    ptr_1 = edges1.begin(), ptr_2 = edges2.begin();
    while (ptr_1 < end_1 && ptr_2 < end_2) {
        if (*ptr_1 == *ptr_2) {
            ++union_size;
            ++ptr_1;
            ++ptr_2;
        } else if (*ptr_1 < *ptr_2) {
            ++union_size;
            ++ptr_1;
        } else {
            ++union_size;
            ++ptr_2;
        }
    }
    while (ptr_1 < end_1) {
        ++union_size;
        ++ptr_1;
    }
    while (ptr_2 < end_2) {
        ++union_size;
        ++ptr_2;
    }
    edge edge = {i, j, int(edges[i].size()), int(edges[j].size()), intersection_size, union_size};
    basic_index.emplace_back(edge);
}

void network::initialize(const std::string &filename) {
    // 初始化网络
    read_csv(filename);
    for (int i = 0; i < vertex_count; ++i) {
        std::vector<int> edge_i = edges[i];
        auto edge_i_end = edge_i.end();
        for (int j = i + 1; j < vertex_count; ++j) {
            if (std::find(edge_i.begin(), edge_i.end(), j) == edge_i_end) {
                get_basic_index(i, j);
            }
        }
    }
}

void network::predict(const std::vector<std::string> &methods, int result_count) {
    // 根据方法进行预测 并指定显示的结果数
    for (const std::string &_method: methods) {
        printf("[预测] 根据 %s 指标\n", _method.c_str());
        std::string method = process_text(_method);
        if (method == "cn") {
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.intersection_size > edge2.intersection_size;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %d\n", edge.start + 1, edge.end + 1, edge.intersection_size);
            });
        } else if (method == "salton") {
            std::for_each(basic_index.begin(), basic_index.end(), predict_methods::calc_salton);
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.salton > edge2.salton;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.salton);
            });
        } else if (method == "jaccard") {
            std::for_each(basic_index.begin(), basic_index.end(), predict_methods::calc_jaccard);
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.jaccard > edge2.jaccard;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.jaccard);
            });

        } else if (method == "sorenson") {
            std::for_each(basic_index.begin(), basic_index.end(), predict_methods::calc_sorenson);
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.sorenson > edge2.sorenson;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.sorenson);
            });
        } else if (method == "hpi") {
            std::for_each(basic_index.begin(), basic_index.end(), predict_methods::calc_HPI);
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.HPI > edge2.HPI;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.HPI);
            });
        } else if (method == "hdi") {
            std::for_each(basic_index.begin(), basic_index.end(), predict_methods::calc_HDI);
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.HDI > edge2.HDI;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.HDI);
            });
        } else if (method == "lhn") {
            std::for_each(basic_index.begin(), basic_index.end(), predict_methods::calc_LHN);
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.LHN > edge2.LHN;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.LHN);
            });
        } else if (method == "pa") {
            std::for_each(basic_index.begin(), basic_index.end(), predict_methods::calc_PA);
            std::sort(basic_index.begin(), basic_index.end(),
                      [&](struct edge &edge1, struct edge &edge2) -> bool {
                          return edge1.PA > edge2.PA;
                      });
            std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                printf(" %d - %d: %d\n", edge.start + 1, edge.end + 1, edge.PA);
            });
        } else if (method == "aa" || method == "ra") {
            std::for_each(basic_index.begin(), basic_index.end(), [&](edge &edge) -> void {
                double res = 0;
                std::vector<int> &edge1 = edges[edge.start], &edge2 = edges[edge.end], intersection;
                std::insert_iterator<std::vector<int>> insert_iterator(intersection, intersection.begin());
                std::set_intersection(edge1.begin(), edge1.end(), edge2.begin(), edge2.end(), insert_iterator);
                if (intersection.empty()) { return; }
                if (method == "aa") {
                    for (int i: intersection) {
                        res += 1.0 / log(edges[i].size());
                    }
                    edge.AA = res;
                } else {
                    for (int i: intersection) {
                        res += 1.0 / double(edges[i].size());
                    }
                    edge.RA = res;
                }
            });
            if (method == "aa") {
                std::sort(basic_index.begin(), basic_index.end(),
                          [&](struct edge &edge1, struct edge &edge2) -> bool {
                              return edge1.AA > edge2.AA;
                          });
                std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                    printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.AA);
                });
            } else {
                std::sort(basic_index.begin(), basic_index.end(),
                          [&](struct edge &edge1, struct edge &edge2) -> bool {
                              return edge1.RA > edge2.RA;
                          });
                std::for_each_n(basic_index.begin(), result_count, [&](struct edge &edge) -> void {
                    printf(" %d - %d: %.3f\n", edge.start + 1, edge.end + 1, edge.RA);
                });
            }
        } else {
            printf(" [错误] 找不到此方法！");
        }
    }
}
