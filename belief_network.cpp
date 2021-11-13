#include <iostream> // for cout
#include <fstream> // for file i/o
#include <string>
#include <math.h> // for exponents

using namespace std;

struct conditional_probablility_table
{
	char **dependency_combinations; // each row will represent a different combination
	int combo_rows, combo_cols;
	float *probabilities; // one for each row of dependency_combinations
	int number_of_probabilties;
};

struct node
{
	char node_name;
	char *dependencies;
	int number_of_dependencies;
	conditional_probablility_table CPT;
};
int node_count;
node *nodes;

struct probablilty
{
	char name_of_probability;
	int number_of_events;
	char *events;
};
int probablilty_count;
probablilty *probabilities_to_calculate;


bool check_dependency(int node_index, char dependency_name, int &dependency_index)
{
	for(int i=0;i<nodes[node_index].number_of_dependencies;i++)
	{
		if(dependency_name == nodes[node_index].dependencies[i])
		{
			dependency_index = i;
			return true;
		}
	}
	return false;
}

bool check_dependency(int node_index, char dependency_name)
{
	for(int i=0;i<nodes[node_index].number_of_dependencies;i++)
	{
		if(dependency_name == nodes[node_index].dependencies[i])
		{
			return true;
		}
	}
	return false;
}

int find_node(char node_name)
{
	for(int i=0;i<node_count;i++)
	{
		if(node_name == nodes[i].node_name)
		{
			return i;
		}
	}
	return 0;
}

bool ReadFile(char* fname)
{
	ifstream inputStream;
	inputStream.open(fname);
	if (inputStream.is_open() == true)
	{
		char c;

		inputStream >> node_count;
		nodes = new node[node_count];

		for(int i=0;i<node_count;i++)
		{
			char current_node_name;
			int dependency_count;
			
			inputStream.get(c);
			while(c == ' ' || c == '\n')
			{
				inputStream.get(c);
			}

			nodes[i].node_name = c;
			inputStream >> nodes[i].number_of_dependencies;
			nodes[i].dependencies = new char[nodes[i].number_of_dependencies];
			for(int j=0; j<nodes[i].number_of_dependencies;j++)
			{
				inputStream.get(c);
				while(c == ' ' || c == '\n')
				{
					inputStream.get(c);
				}
				//fscanf(file, "%c", &dependency);
				nodes[i].dependencies[j] = c;
			}

			// 2^dependency_count rows for each dependency combo
			nodes[i].CPT.combo_rows = pow(2,nodes[i].number_of_dependencies);
			nodes[i].CPT.combo_cols = nodes[i].number_of_dependencies;

			nodes[i].CPT.dependency_combinations = new char*[nodes[i].CPT.combo_rows];
			for(int rows=0;rows<nodes[i].CPT.combo_rows;rows++)
			{
				nodes[i].CPT.dependency_combinations[rows] = new char[nodes[i].CPT.combo_cols];
				for (int cols=0; cols < nodes[i].CPT.combo_cols; cols++)
				{
					nodes[i].CPT.dependency_combinations[rows][cols] = '-';
				}
			}
			nodes[i].CPT.number_of_probabilties = nodes[i].CPT.combo_rows;
			if(nodes[i].number_of_dependencies == 0)
			{
				nodes[i].CPT.number_of_probabilties = 1;
			}
			nodes[i].CPT.probabilities = new float[nodes[i].CPT.number_of_probabilties];
			for(int j=0; j<nodes[i].CPT.number_of_probabilties;j++)
			{
				nodes[i].CPT.probabilities[j] = -1.0;
			}

			if(nodes[i].number_of_dependencies == 0)
			{
				/*inputStream.get(c);
				while(c!=nodes[i].node_name)
				{
					inputStream.get(c);
				}
				while(c!='0' && c!='1')
				{
					if(c=='0')
					{
							
					}
					if(c=='1')
					{
							
					}
				}*/
				inputStream.get(c);
				while(c!='=')
				{
					inputStream.get(c);	
				}
				//twice 
				inputStream.get(c);
				while(c!='=')
				{
					inputStream.get(c);	
				}
				inputStream >> nodes[i].CPT.probabilities[0];
			}
			else
			{
				for(int k=0;k<nodes[i].CPT.combo_rows;k++)
				{
					/*inputStream.get(c);
					while(c!=nodes[i].node_name)
					{
						inputStream.get(c);
					}
					while(c!='0' && c!='1')
					{
						if(c=='0')
						{
								
						}
						if(c=='1')
						{
								
						}

					}*/

					for(int j=0;j<nodes[i].number_of_dependencies;j++)
					{
						int dependency_index;
						inputStream.get(c);
						while(!check_dependency(i, c, dependency_index))
						{
							inputStream.get(c);
						}
						inputStream.get(c);
						while(c!='0' && c!='1')
						{
							inputStream.get(c);	
						}
						if(c=='0')
						{
							nodes[i].CPT.dependency_combinations[k][dependency_index] = 'F';	
						}
						else if(c=='1')
						{
							nodes[i].CPT.dependency_combinations[k][dependency_index] = 'T';	
						}
						
					}
					inputStream.get(c);
					while(c!='=')
					{
						inputStream.get(c);	
					}
					inputStream >> nodes[i].CPT.probabilities[k];
				}

			}
		}

		// code to read probabilities that need to be calculated
		inputStream >> probablilty_count;
		probabilities_to_calculate = new probablilty[probablilty_count];
		for(int i=0;i<probablilty_count;i++)
		{
			inputStream >> probabilities_to_calculate[i].number_of_events;
			inputStream.get(c);
			while(c!='(')
			{
				inputStream.get(c);
			}
			inputStream.get(c);
			probabilities_to_calculate[i].name_of_probability = c;
			probabilities_to_calculate[i].events = new char[probabilities_to_calculate[i].number_of_events];

			for(int j=0; j<probabilities_to_calculate[i].number_of_events;j++)
			{
				inputStream.get(c);
				while(c!='|' && c!=',')
				{
					inputStream.get(c);
				}
				inputStream.get(c);
				probabilities_to_calculate[i].events[j] = c;
			}
			inputStream.get(c);
			while(c!=')')
			{
				inputStream.get(c);
			}
		}

		inputStream.close();
		return true;
	}
	else
	{
		printf("Failed to read file...\n");
		return false;
	}
}

void print_network()
{
	cout << "Belief Network:" << endl;
	for(int i=0;i<node_count;i++)
	{
		for(int j=0;j<nodes[i].number_of_dependencies;j++)
		{
			cout << nodes[i].dependencies[j] << " -> " << nodes[i].node_name << endl;
		}
	}
}

void print_cpt()
{
	for(int i=0;i<node_count;i++)
	{
		cout << "CPT for node " << nodes[i].node_name << ":" << endl;

		if(nodes[i].number_of_dependencies == 0)
		{
			cout << "P(" << nodes[i].node_name << ")" << endl;
			cout << nodes[i].CPT.probabilities[0] << endl;
		}
		else
		{
			for(int j=0;j<nodes[i].number_of_dependencies;j++)
			{
				cout << nodes[i].dependencies[j] << "\t";
			}

			cout << "P(" << nodes[i].node_name << ")" << endl;

			for(int j=0;j<nodes[i].CPT.combo_rows;j++)
			{
				for(int k=0;k<nodes[i].CPT.combo_cols;k++)
				{
					cout << nodes[i].CPT.dependency_combinations[j][k] << "\t";
				}
				cout << nodes[i].CPT.probabilities[j] << endl;
			}
		}
	}
}

void print_probabilities()
{
	for(int i=0;i<probablilty_count;i++)
	{
		if(probabilities_to_calculate[i].number_of_events > 0)
		{
			//cout << "P(" << probabilities_to_calculate[i].name_of_probability << "|" << probabilities_to_calculate[i].event << ")" << endl;
			cout << "P(" << probabilities_to_calculate[i].name_of_probability << "|";
			for(int j=0;j<probabilities_to_calculate[i].number_of_events-1;j++)
			{
				cout << probabilities_to_calculate[i].events[j] << ",";
			}
			cout << probabilities_to_calculate[i].events[probabilities_to_calculate[i].number_of_events-1] << ")" << endl;
		}
		else
		{
			cout << "P(" << probabilities_to_calculate[i].name_of_probability << ")" << endl;
		}

	}
}

void print_probability(int i)
{
	if(probabilities_to_calculate[i].number_of_events > 0)
	{
		//cout << "P(" << probabilities_to_calculate[i].name_of_probability << "|" << probabilities_to_calculate[i].event << ")" << endl;
		cout << "P(" << probabilities_to_calculate[i].name_of_probability << "|";
		for(int j=0;j<probabilities_to_calculate[i].number_of_events-1;j++)
		{
			cout << probabilities_to_calculate[i].events[j] << ",";
		}
		cout << probabilities_to_calculate[i].events[probabilities_to_calculate[i].number_of_events-1] << ")" << endl;
	}
	else
	{
		cout << "P(" << probabilities_to_calculate[i].name_of_probability << ")" << endl;
	}

}
// currently will only solve P(A|B), P(A), and P(B|A) and the node structure must be the same as the one specified in the prompt
void solve_probabilities()
{
	for(int i=0;i<probablilty_count;i++)
	{
		print_probability(i);
		int node_index = find_node(probabilities_to_calculate[i].name_of_probability);
		if(node_index == -1)
		{
			cout << "Error: node does not exist in network." << endl;
		}
		// P(A)
		else if(probabilities_to_calculate[i].number_of_events == 0)
		{
			cout << "= " << "\t" << "P(" << nodes[node_index].node_name << "|~" << nodes[node_index].dependencies[0] << ",~" << nodes[node_index].dependencies[1] << ")*P(~" << nodes[node_index].dependencies[0] << ")P(~" << nodes[node_index].dependencies[1] << ") +" << endl;
			cout << "\t" << "P(" << nodes[node_index].node_name << "|~" << nodes[node_index].dependencies[0] << "," << nodes[node_index].dependencies[1] << ")*P(~" << nodes[node_index].dependencies[0] << ")P(" << nodes[node_index].dependencies[1] << ") +" << endl;
			cout << "\t" << "P(" << nodes[node_index].node_name << "|" << nodes[node_index].dependencies[0] << ",~" << nodes[node_index].dependencies[1] << ")*P(" << nodes[node_index].dependencies[0] << ")P(~" << nodes[node_index].dependencies[1] << ") +" << endl;
			cout << "\t" << "P(" << nodes[node_index].node_name << "|" << nodes[node_index].dependencies[0] << "," << nodes[node_index].dependencies[1] << ")*P(" << nodes[node_index].dependencies[0] << ")P(" << nodes[node_index].dependencies[1] << ")" << endl;

			float falsefalse, falsetrue, truefalse, truetrue;
			for(int j=0;j<nodes[node_index].CPT.combo_rows;j++)
			{
				if(nodes[node_index].CPT.dependency_combinations[j][0] == 'F' &&
				   nodes[node_index].CPT.dependency_combinations[j][1] == 'F')
				{
					falsefalse = nodes[node_index].CPT.probabilities[j];
				}
				else if(nodes[node_index].CPT.dependency_combinations[j][0] == 'F' &&
				   nodes[node_index].CPT.dependency_combinations[j][1] == 'T')
				{
					falsetrue = nodes[node_index].CPT.probabilities[j];
				}
				else if(nodes[node_index].CPT.dependency_combinations[j][0] == 'T' &&
				   nodes[node_index].CPT.dependency_combinations[j][1] == 'F')
				{
					truefalse = nodes[node_index].CPT.probabilities[j];
				}
				else if(nodes[node_index].CPT.dependency_combinations[j][0] == 'T' &&
				   nodes[node_index].CPT.dependency_combinations[j][1] == 'T')
				{
					truetrue = nodes[node_index].CPT.probabilities[j];
				}
			}

			int first_index = find_node(nodes[node_index].dependencies[0]);
			int second_index = find_node(nodes[node_index].dependencies[1]);

			cout << "= " << "\t" << falsefalse << "*(1-" << nodes[first_index].CPT.probabilities[0] << ")*(1-" << nodes[second_index].CPT.probabilities[0] << ")+" << endl;
			cout << "\t" << falsetrue << "*(1-" << nodes[first_index].CPT.probabilities[0] << ")*" << nodes[second_index].CPT.probabilities[0] << "+" << endl;
			cout << "\t" << truefalse << "*" << nodes[first_index].CPT.probabilities[0] << "*(1-" << nodes[second_index].CPT.probabilities[0] << ")+" << endl;
			cout << "\t" << truefalse << "*" << nodes[first_index].CPT.probabilities[0] << "*" << nodes[second_index].CPT.probabilities[0] << endl;

			cout << "= " << "\t" << falsefalse*(1-nodes[first_index].CPT.probabilities[0])*(1-nodes[second_index].CPT.probabilities[0])+
			falsetrue*(1-nodes[first_index].CPT.probabilities[0])*(nodes[second_index].CPT.probabilities[0])+
			truefalse*(nodes[first_index].CPT.probabilities[0])*(1-nodes[second_index].CPT.probabilities[0])+
			truetrue*(nodes[first_index].CPT.probabilities[0])*(nodes[second_index].CPT.probabilities[0]) << endl << endl;
		}
		// P(A|B)
		else if(check_dependency(node_index, probabilities_to_calculate[i].events[0]))
		{
			cout << "= " << "\t" << "P(" << nodes[node_index].node_name << "|" << nodes[node_index].dependencies[0] << "," << nodes[node_index].dependencies[1] << ")*P(" << nodes[node_index].dependencies[1] << ")+("<<nodes[node_index].node_name << "|" << nodes[node_index].dependencies[0] << ",~" << nodes[node_index].dependencies[1] << ")*P(~" << nodes[node_index].dependencies[1] << ")" << endl;

			float _true, _false;
			int event_index = find_node(probabilities_to_calculate[i].events[0]);

			if(event_index == 0)
			{
				for(int j=0;j<nodes[node_index].CPT.combo_rows;j++)
				{
					if(nodes[node_index].CPT.dependency_combinations[j][1] == 'F')
					{
						_false = nodes[node_index].CPT.probabilities[j];
					}
					else if(nodes[node_index].CPT.dependency_combinations[j][1] == 'T')
					{
						_true = nodes[node_index].CPT.probabilities[j];
					}	
				}
				cout << "= " << "\t" << _true<< "*" <<nodes[find_node(nodes[node_index].dependencies[1])].CPT.probabilities[0] << " + " << _false << "*(1-" << nodes[find_node(nodes[node_index].dependencies[1])].CPT.probabilities[0] << ")" << endl;
				cout << "= " << "\t" << _true*nodes[find_node(nodes[node_index].dependencies[1])].CPT.probabilities[0] + _false*(1-nodes[find_node(nodes[node_index].dependencies[1])].CPT.probabilities[0]) << endl << endl;
			}
			if(event_index == 1)
			{
				for(int j=0;j<nodes[node_index].CPT.combo_rows;j++)
				{
					if(nodes[node_index].CPT.dependency_combinations[j][0] == 'F')
					{
						_false = nodes[node_index].CPT.probabilities[j];
					}
					else if(nodes[node_index].CPT.dependency_combinations[j][0] == 'T')
					{
						_true = nodes[node_index].CPT.probabilities[j];
					}	
				}
				cout << "= " << "\t" << _true<< "*" <<nodes[find_node(nodes[node_index].dependencies[0])].CPT.probabilities[0] << " + " << _false << "*(1-" << nodes[find_node(nodes[node_index].dependencies[0])].CPT.probabilities[0] << ")" << endl;
				cout << "= " << "\t" << _true*nodes[find_node(nodes[node_index].dependencies[0])].CPT.probabilities[0] + _false*(1-nodes[find_node(nodes[node_index].dependencies[0])].CPT.probabilities[0]) << endl << endl;
			}
		}
		// P(B|A)
		else if(check_dependency(find_node(probabilities_to_calculate[i].events[0]), nodes[node_index].node_name))
		{
			cout << "= " << "\t" << "P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|" << nodes[node_index].node_name << ")*P(" << nodes[node_index].node_name << ") / P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << ")" << endl;


			cout << "P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|" << nodes[node_index].node_name << ") = P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << "," << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")*P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")+("<<nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << ",~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")*P(~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")" << endl;


			cout << "= " << "\t" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[3] << "*" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] << "*" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[2] << "*(1-" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] << ")" << endl;

			cout << "= " << "\t" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[3] * nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] + nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[2] * (1 - nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0]) << endl;

			float var1 = nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[3] * nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] + nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[2] * (1 - nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0]);

			cout << "P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << ")" << endl;
			cout << "= " << "\t" << "P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << ",~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")*P(~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << ")P(~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ") +" << endl;
			cout << "\t" << "P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << "," << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")*P(~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << ")P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ") +" << endl;
			cout << "\t" << "P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << ",~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")*P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << ")P(~" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ") +" << endl;
			cout << "\t" << "P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].node_name << "|" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << "," << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")*P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0] << ")P(" << nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1] << ")" << endl;


			cout << "= " << "\t" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[0] << "*(1-" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0] << ")*(1-" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] << ")+" << endl;
			cout << "\t" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[1] << "*(1-" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0] << ")*" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] << "+" << endl;
			cout << "\t" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[2] << "*" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0] << "*(1-" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] << ")+" << endl;
			cout << "\t" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[3] << "*" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0] << "*" << nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0] << endl;


			cout << "= " << "\t" << nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[0] * (1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0])+
			nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[1] * (1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0])+
			nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[2] * (nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0])+
			nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[3] * (nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0]) << endl << endl;

			float var2 =  nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[0] * (1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0])+
			nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[1] * (1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0])+
			nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[2] * (nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(1-nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0])+
			nodes[find_node(probabilities_to_calculate[i].events[0])].CPT.probabilities[3] * (nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[0])].CPT.probabilities[0])*(nodes[find_node(nodes[find_node(probabilities_to_calculate[i].events[0])].dependencies[1])].CPT.probabilities[0]);

			cout << "P(" << probabilities_to_calculate[i].name_of_probability << "|" << probabilities_to_calculate[i].events[0] << ") = " << var1 << "*" << nodes[find_node(probabilities_to_calculate[i].name_of_probability)].CPT.probabilities[0] << "/" << var2 << endl;

			cout << "= " << "\t" << var1 * nodes[find_node(probabilities_to_calculate[i].name_of_probability)].CPT.probabilities[0] / var2 << endl << endl;
		}
	}
}
/*void solve_probabilities()
{
	for(int i=0;i<probablilty_count;i++)
	{
		print_probability(i);
		//fuction to search for name_of_probability that matches node_name, returns index of node_name
		for(int j=0;j<node_count;j++)
		{
			if(probabilities_to_calculate[i].name_of_probability == nodes[j].node_name)
			{
				if(nodes[j].number_of_dependencies == 0 &&
				   probabilities_to_calculate[i].number_of_events == 0)
				{
					cout << "P(" << probabilities_to_calculate[i].name_of_probability << ") = " << nodes[j].CPT.probabilities[0] << endl;
				}
			}
			else if(nodes[j].number_of_dependencies != 0)
			{
				for(int k=0;k<pow(2,probabilities_to_calculate[i].number_of_events);k++)
				{
					if()
					{

					}
				}

			}
		}
		cout << endl;
	}
}*/

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		if (!ReadFile(argv[1]))
			return 2;
	}
	else
	{
		printf("No file argument given...\n");
		return 1;
	}

	print_network();
	cout << endl;
	print_cpt();
	cout << endl;
	//print_probabilities();
	//cout << endl;
	solve_probabilities();

	return 0;
}
