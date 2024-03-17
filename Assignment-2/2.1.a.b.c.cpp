#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846
#define MAX 1000000.0

typedef struct {
    double x;
    double y;
} Point;

Point* generate_points_on_circle(int num_points, double radius, Point center) {
    Point* points = (Point*)malloc(num_points * sizeof(Point));
    double angle_step = 2 * PI / num_points;
    for (int i = 0; i < num_points; i++) {
        double angle = i * angle_step;
        points[i].x = center.x + radius * cos(angle);
        points[i].y = center.y + radius * sin(angle);
    }
    return points;
}

double cross_product(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

int compare(const void* vp1, const void* vp2) {
    Point *p1 = (Point*)vp1;
    Point *p2 = (Point*)vp2;
    double angle1 = atan2(p1->y, p1->x);
    double angle2 = atan2(p2->y, p2->x);
    if (angle1 < angle2) return -1;
    if (angle1 > angle2) return 1;
    return 0;
}

double min(double x, double y)
{
    return (x <= y) ? x : y;
}

// A utility function to find distance between two points in a plane
double dist(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) +
                (p1.y - p2.y)*(p1.y - p2.y));
}


double cost(Point points[], int i, int j, int k)
{
    Point p1 = points[i], p2 = points[j], p3 = points[k];
    return dist(p1, p2) + dist(p2, p3) + dist(p3, p1);
}


double mTC(Point points[], int i, int j)
{
    if (j < i + 2)
        return 0;

    double res = MAX;

    for (int k = i + 1; k < j; k++)
        res = min(res, (mTC(points, i, k) + mTC(points, k, j) + cost(points, i, k, j)));
    return res;
}


double mTCDP(Point points[], int n) {
    if (n < 3)
        return 0;

   
    double table[n][n];

    
    for (int gap = 0; gap < n; gap++) {
        for (int i = 0, j = gap; j < n; i++, j++) {
            if (j < i + 2)
                table[i][j] = 0.0;
            else {
                table[i][j] = MAX;
                for (int k = i + 1; k < j; k++) {
                    double val = table[i][k] + table[k][j] + cost(points, i, j, k);
                    if (table[i][j] > val)
                        table[i][j] = val;
                }
            }
        }
    }
    return table[0][n - 1];
}

Point* convex_hull(Point* points, int num_points, int* num_vertices) {
    if (num_points < 3) {
        return NULL;
    }

    // Sort points by polar angle with respect to the lowest point
    qsort(points, num_points, sizeof(Point), compare);

    // Initialize the convex hull as empty
    Point* hull = (Point*)malloc(num_points * sizeof(Point));
    int hull_size = 0;

    // Build the convex hull using Graham scan
    for (int i = 0; i < num_points; i++) {
        while (hull_size >= 2 && cross_product(hull[hull_size - 2], hull[hull_size - 1], points[i]) <= 0) {
            hull_size--;
        }
        hull[hull_size++] = points[i];
    }

    *num_vertices = hull_size;
    return hull;
}

Point* generate_convex_polygon_from_circle(int num_points, int num_vertices) {
    Point center = {0.0, 0.0};
    double radius = 10.0;
    Point* points_on_circle = generate_points_on_circle(num_points, radius, center);
    Point* selected_points = (Point*)malloc(num_vertices * sizeof(Point));
    for (int i = 0; i < num_vertices; i++) {
        selected_points[i] = points_on_circle[rand() % num_points];
    }
    int num_vertices_hull;
    Point* convex_polygon = convex_hull(selected_points, num_vertices, &num_vertices_hull);
    free(points_on_circle);
    free(selected_points);
    return convex_polygon;
}

void print_polygon(Point* polygon, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        printf("(%f, %f) ", polygon[i].x, polygon[i].y);
    }
    printf("\n");
}

int main() {
    int num_polygons = 10;
    int min_vertices = 3;
    int max_vertices = 10;
    int num_points = 100;
    srand(time(NULL));

    for (int i = 0; i < num_polygons; i++) {
        int num_vertices = rand() % (max_vertices - min_vertices + 1) + min_vertices;
        Point* convex_polygon = generate_convex_polygon_from_circle(num_points, num_vertices);
        if (convex_polygon) {
            printf("Convex Polygon %d: ", i + 1);
            print_polygon(convex_polygon, num_vertices);
            double min_cost_rec = mTC(convex_polygon, 0, num_vertices - 1);
            double min_cost_dp = mTCDP(convex_polygon, num_vertices);
            printf("Minimum cost for Convex Polygon %d (Recursive): %.2f\n", i + 1, min_cost_rec);
            printf("Minimum cost for Convex Polygon %d (Dynamic Programming): %.2f\n", i + 1, min_cost_dp);
            free(convex_polygon);
        }
    }

    // Save polygon vertices, minimum cost,and both results to a csv file
    FILE* file = fopen("convex_polygons_results.csv", "w");
    if (file) {
        fprintf(file, "Polygon,Vertices,Cost (Recursive),Cost (Dynamic Programming)\n");
        for (int i = 0; i < num_polygons; i++) {
            int num_vertices = rand() % (max_vertices - min_vertices + 1) + min_vertices;
            Point* convex_polygon = generate_convex_polygon_from_circle(num_points, num_vertices);
            if (convex_polygon) {

                double min_cost_rec = mTC(convex_polygon, 0, num_vertices - 1);
            
                double min_cost_dp = mTCDP(convex_polygon, num_vertices);
                fprintf(file, "%d,", i + 1);
                for (int j = 0; j < num_vertices; j++) {
                    fprintf(file, "(%f, %f)", convex_polygon[j].x, convex_polygon[j].y);
                    if (j < num_vertices - 1) {
                        fprintf(file, ",");
                    }
                }
                fprintf(file, ",%.2f,%.2f\n", min_cost_rec, min_cost_dp);
                free(convex_polygon);
            }
        }
        fclose(file);
    }

    return 0;
}
