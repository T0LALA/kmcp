#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Класс для представления точек
class Point {
public:
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {}

    double distance(shared_ptr<Point> other) const {
        double dx = x - other->x;
        double dy = y - other->y;
        return sqrt(dx * dx + dy * dy);
    }
};

// Класс для представления кластеров
class Cluster {
public:
    shared_ptr<Point> centroid;
    vector<Point> points;
    Cluster()
    {
        centroid = make_shared<Point>(0, 0);
    }
    ~Cluster()
    {
        points.clear();
    }
    void AddPoint(Point& point) {
        points.push_back(point);
        _recalculatedCentroids(point);
    }
    void AddPoint(vector<Point>& ppoints) {
        for (size_t i = 0; i < ppoints.size(); i++)
            points.push_back(ppoints[i]);
        _recalculatedCentroids(ppoints[ppoints.size()-1]);
    }
    bool Empty() {
        return points.empty();
    }
private:
    void _recalculatedCentroids(Point& point) {
        if (points.empty()) {
            centroid->x = point.x;
            centroid->y = point.y;
        }
        else {
            double x = 0;
            double y = 0;
            for (Point point : points) {
               x += point.x;
               y += point.y;
            }
            centroid->x = x / points.size();
            centroid->y = y / points.size();
        }
          
    }
};
int IdentifyPointCluster(Cluster& cluster1, Cluster& cluster2, Point& point) {
    if (point.distance(cluster1.centroid) < point.distance(cluster2.centroid))
        return 1;
    else
        return 2;
}

int main() {
    // Пример данных
    vector<Point> data = { Point(1, 2), Point(2, 3), Point(3, 4), Point(5, 6), Point(6, 7), Point(7, 8) };
    const double distance = 4; //радиус кластера

    vector<Cluster> clusters;
    Cluster firstCluster = Cluster();
    firstCluster.AddPoint(data[0]);
    clusters.push_back(firstCluster);
    for (size_t i = 1; i < data.size(); i++) {
        int min_j = -1;
        double minDistance = data[i].distance(clusters[0].centroid);
        if (minDistance <= distance)
            min_j = 0;
        for (size_t j = 1; j < clusters.size(); j++) {
            if (data[i].distance(clusters[j].centroid) < minDistance) {
                minDistance = data[i].distance(clusters[j].centroid);
                min_j = j;
            }
        }
        if (min_j != -1 && minDistance <= distance) 
            clusters[min_j].AddPoint(data[i]);
        else {
            clusters.push_back(Cluster());
            clusters[clusters.size()-1].AddPoint(data[i]);
        }
    }

    // Вывод результата кластеризации
    int clusterNumber = 1;
    for (const Cluster& cluster : clusters) {
        cout << "Cluster " << clusterNumber << " (Centroid: " << cluster.centroid->x << ", " << cluster.centroid->y << "):" << endl;
        for (const Point& point : cluster.points) {
            cout << "(" << point.x << ", " << point.y << ")" << endl;
        }
        cout << endl;
        clusterNumber++;
    }
    clusters.clear();


    //То что надо было, 2 группы точек, появляется новая
    vector<Point> cluster1 = { Point(1, 2), Point(2, 3), Point(3, 4), Point(5, 6) };
    vector<Point> cluster2 = { Point(11, 12), Point(13, 14), Point(14, 15), Point(15, 16) };
    Cluster secondCluster = Cluster();
    secondCluster.AddPoint(cluster1);
    Cluster thirdCluster = Cluster();
    thirdCluster.AddPoint(cluster2);
    Point point = Point(0, 0);
    cout << IdentifyPointCluster(secondCluster, thirdCluster, point) << endl;
    cluster1.clear();
    cluster2.clear();
    return 0;
}
