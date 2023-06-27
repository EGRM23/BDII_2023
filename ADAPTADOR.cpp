#include <iostream>
#include <fstream>
using namespace std;

void adaptador_archivo()
{
	char letra;
	char* dato;
	
	string tipo;
	int tam_bytes[12];
	int var = 0;
	int pos = 0;
	bool detectcoma = true;
	ofstream nuevoesquema("titanic_esquema.txt");
	ofstream nuevabase("titanic.txt");
	ifstream modelo("titanic.csv");
	
	cout << "Ingrese el tipo de dato de las siguientes variables:\n";
	nuevoesquema << "titanic#";
	while (modelo >> noskipws >> letra)
	{
		if(nuevoesquema.is_open())
		{
			if (letra == '\n')
			{
				cout << ": ";
				cin >> tipo;
				cout << "Tam. Bytes: ";
				cin >> tam_bytes[var];
				nuevoesquema << "#" << tipo << "[" << tam_bytes[var] << "]\n";
				var = 0;

				dato = new char[tam_bytes[var]*8+1];
				nuevoesquema.close();
			}
			else if (letra == ',')
			{
				cout << ": ";
				cin >> tipo;
				cout << "Tam. Bytes: ";
				cin >> tam_bytes[var]; 
				nuevoesquema << "#" << tipo << "[" << tam_bytes[var++] << "]#";
			}
			else
			{
				cout << letra;
				nuevoesquema << letra;
			}
		}
		else
		{
			if(letra == '"') detectcoma = !detectcoma;
			
			if ((letra == ',' || letra == '\n' || letra == EOF) && detectcoma)
			{
				if (modelo.peek() == EOF)
				{
					dato[pos++] = letra;
					for(int i = 0; i < tam_bytes[var]*8; i++)
					{
						if (i < pos) nuevabase << dato[i];
						else nuevabase << ' ';
					}
					delete dato;
					break;
				}
				
				for(int i = 0; i < tam_bytes[var]*8; i++)
				{
					if (i < pos) nuevabase << dato[i];
					else nuevabase << ' ';
				}
				
				delete dato;
				if (letra == ',') var++;
				else if (letra == '\n') var = 0;
				else if (letra == EOF) break;

				dato = new char[tam_bytes[var]*8];
				pos = 0;
			}
			else
			{
				dato[pos++] = letra;
			}
		}
	}
	modelo.close();
	nuevabase.close();
}

int main()
{
	adaptador_archivo();
	return 0;
}