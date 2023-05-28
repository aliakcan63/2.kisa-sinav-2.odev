#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 7
#define EPSILON 10
#define MIN_PTS 3

struct Point {
    double x;
    double y;
    int cluster;
    double coreDistance;
    double reachabilityDistance;
};

double euclideanDistance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

double coreDistance(struct Point* points, int pointIndex) {
    int neighborCount = 0;
    double distanceSum = 0;

    for (int i = 0; i < N; ++i) {
        if (i != pointIndex) {
            double distance = euclideanDistance(points[pointIndex], points[i]);
            if (distance <= EPSILON) {
                ++neighborCount;
                distanceSum += distance;
            }
        }
    }

    if (neighborCount >= MIN_PTS) {
        return distanceSum / neighborCount;
    } else {
        return -1;   // çekirdek mesafesi tanımsız
    }
}

double reachabilityDistance(struct Point* points, int pointIndex, int neighborIndex) {
    double distance = euclideanDistance(points[pointIndex], points[neighborIndex]);
    double coreDist = points[pointIndex].coreDistance;

    if (coreDist == -1) {
        return -1;  // Ulaşılabilirlik mesafesi tanımsız
    } else {
        return fmax(distance, coreDist);
    }
}

void opticsAlgorithm(struct Point* points) {
    for (int i = 0; i < N; ++i) {
        points[i].coreDistance = coreDistance(points, i);
    }

    for (int i = 0; i < N; ++i) {
        double reachDist = -1;  // Ulaşılabilirlik mesafesi tanımsız

        for (int j = 0; j < N; ++j) {
            if (i != j) {
                double newReachDist = reachabilityDistance(points, i, j);
                if (newReachDist != -1 && (reachDist == -1 || newReachDist < reachDist)) {
                    reachDist = newReachDist;
                }
            }
        }

        points[i].reachabilityDistance = reachDist;
    }
}

int main() {
    struct Point points[N] = {
        {516.0127058, 393.0145139, 0, 0.0, 0.0},
        {436.2117622, 408.6565849, 0, 0.0, 0.0},
        {512.0526012, 372.0220136, 0, 0.0, 0.0},
        {489.1404645, 401.8071594, 0, 0.0, 0.0},
        {446.2079859, 338.5166822, 0, 0.0, 0.0},
        {516.4143943, 354.1946332, 0, 0.0, 0.0},
        {499.3863531, 414.4322961, 0, 0.0, 0.0}
    };

    opticsAlgorithm(points);

    // En küçük ve en büyük elemanın olduğu küme
    int minIndex = 0;
    int maxIndex = 0;
    for (int i = 1; i < N; ++i) {
        if (points[i].x < points[minIndex].x || (points[i].x == points[minIndex].x && points[i].y < points[minIndex].y)) {
            minIndex = i;
        }
        if (points[i].x > points[maxIndex].x || (points[i].x == points[maxIndex].x && points[i].y > points[maxIndex].y)) {
            maxIndex = i;
        }
    }
    points[minIndex].reachabilityDistance = 2;
    points[maxIndex].reachabilityDistance = 2;

    // Diğer elemanlar
    for (int i = 0; i < N; ++i) {
        if (points[i].reachabilityDistance == -1) {
            points[i].reachabilityDistance = 1;
        }
    }

    for (int i = 0; i < N; ++i) {
        printf("Point %.4f, %.4f - Reachability Distance: %.4f\n", points[i].x, points[i].y, points[i].reachabilityDistance);
    }

    return 0;
}






