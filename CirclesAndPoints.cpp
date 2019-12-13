#include <iostream>
#include<math.h>
#include<vector>

using namespace std;

class Point
{
	public:
		double x, y;
		Point(double x,double y)
		{
			this->x = x;
			this->y = y;
		}
		void print()
		{
			cout <<"x = "<< this->x << "\ty = " << this->y << endl;
		}
};

double get_distance(const Point* a, const Point* b)
{
	return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

class Triangle
{
	public:
		Point* a;
		Point* b;
		Point* c;
		Point* centre;
		double R;
		Triangle(Point* a,Point* b,Point* c) 
		{
			this->a = a;
			this->b = b;
			this->c = c;
			double ka = (b->x - a->x) / (a->y - b->y);
			double kc = (c->x - a->x) / (a->y - c->y);
			double ba = (pow(a->x, 2) - pow(b->x, 2) + pow(a->y, 2) - pow(b->y, 2)) / (2 * a->y - 2 * b->y);
			double bc = (pow(a->x, 2) - pow(c->x, 2) + pow(a->y, 2) - pow(c->y, 2)) / (2 * a->y - 2 * c->y);
			if(isinf(abs(ka))&& isinf(abs(kc)))
			{
				if (a->x<c->x && b->x>c->x)
					this->centre = new Point((a->x + b->x) / 2, (a->y + b->y) / 2);
				else if(b->x<a->x && c->x>a->x)
					this->centre = new Point((a->x + c->x) / 2, (a->y + c->y) / 2);
				else
					this->centre = new Point((a->x + c->x) / 2, (a->y + c->y) / 2);
			}				
			else if (isinf(abs(ka)))
				this->centre = new Point((b->x + a->y) / 2, kc * (b->x + a->y) / 2 + bc);
			else if(isinf(abs(kc)))
				this->centre = new Point((c->x + a->y) / 2, ka * (c->x + a->y) / 2 + ba);
			else
			this->centre = new Point((bc - ba) / (ka - kc), ka * (bc - ba) / (ka - kc) + ba);
			this->R = get_distance(centre, a);
		}
		bool get_in_area(Point* p)
		{
			if (!(pow(p->x - this->centre->x, 2) + pow(p->y - this->centre->y, 2) > pow(this->R, 2)))
				return true;
			return false;
		}
};

Point* get_middle_point(const Point* a, const Point* b)
{
	return new Point((a->x + b->x) / 2.0, (a->y + b->y) / 2.0);
}

bool included(Triangle* T, unsigned int nuber_of_points, vector<Point*>&data_mass)
{
	if (nuber_of_points == 0)
		return true;
	else
	{
		Point* p = data_mass[0];
		double min = get_distance(T->centre,data_mass[0]);
		int counter = 0;
		for(int i=1;i<data_mass.size();i++)
			if (get_distance(T->centre, data_mass[i]) < min)
			{
				p = data_mass[i];
				min = get_distance(T->centre,p);
				counter = i;
			}
		data_mass.erase(data_mass.begin()+counter);
		return T->get_in_area(p) & included(T, nuber_of_points - 1, data_mass);
	}
}

void find_radius_and_centre(unsigned int nuber_of_points, vector<Point*>& data_mass)
{
	if (data_mass.size() == 0)
	{
		cout << "You have no points" << endl;
		return;
	}
	if(nuber_of_points>data_mass.size())
	{
		cout << "You haven`t enough points" << endl;
		return;
	}
	switch (nuber_of_points)
	{
		case 0:
		{
			cout << "Incorrect number!" << endl;
			break;
		}
		case 1:
		{
			cout << "Minimal R = 0 for every point : " << endl;
			for (Point* i : data_mass)
				i->print();
			break;
		}
		case 2:
		{
			double min_distanse = get_distance(data_mass[0], data_mass[1]);
			Point* o = get_middle_point(data_mass[0], data_mass[1]);
			for (int i = 0; i < data_mass.size() - 1; i++)
				for (int j = i + 1; j < data_mass.size(); j++)
					if (get_distance(data_mass[i], data_mass[j]) < min_distanse)
					{
						min_distanse = get_distance(data_mass[i], data_mass[j]);
						o = get_middle_point(data_mass[i], data_mass[j]);
					}
			cout << "Minimal R = " << min_distanse / 2.0 << endl << "Circle cetre is in\t"; 
			o->print();
			break;
		}
		default:
		{
			Triangle* mintriangle=nullptr;
			for (int i = 0; i < data_mass.size() - 2; i++)
			{
				for (int j = i + 1; j < data_mass.size() - 1; j++)
				{
					for (int k = j + 1; k < data_mass.size(); k++)
					{
						Triangle* current = new Triangle(data_mass[i], data_mass[j], data_mass[k]);
						vector<Point*>clone;
						for (int m = 0; m < data_mass.size(); m++)
							if (!(m == i || m == j || m == k))
								clone.push_back(data_mass[m]);
						if(included(current,nuber_of_points-3,clone))
							if (mintriangle == nullptr)
								mintriangle = current;
							else
							{
								if (current->R < mintriangle->R)
									mintriangle = current;
							}
					}
				}
			}
			cout << "Minimal R = " << mintriangle->R << endl << "Circle cetre is in\t";
			mintriangle->centre->print();
		}
	}
}

int main()
{
	vector<Point*>map;
	map.push_back(new Point(0,0));
	map.push_back(new Point(-2, 1));
	map.push_back(new Point(0,3));
	map.push_back(new Point(2, 1));
	map.push_back(new Point(2, -2));
	map.push_back(new Point(5, 1));
	map.push_back(new Point(0, 1));


	
	find_radius_and_centre(5,map);
	cin.get();
	return 0;
}

