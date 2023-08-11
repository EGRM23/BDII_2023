#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

class sector
{
public:
	int capacidad;
	int cant_sect;
	sector() {}
	sector(int sectores,int almacenamiento) : cant_sect(sectores),capacidad(almacenamiento) {}
};

class pista
{
public:
	int cant_sect;
	int cant_pistas;
	pista() {}
	pista(int pistas,int sectores) : cant_pistas(pistas),cant_sect(sectores) {}
	long long capacidad(int tam) {return cant_sect*tam;}
};

class plato
{
public:
	long long cant_sect;
	int cant_platos;
	plato() {};
	plato(int platos,int pistas,int sectores) : cant_platos(platos),cant_sect(pistas*sectores*2) {}
	long long capacidad(int tam) {return cant_sect*tam;}
};

class disco
{
public:
	long long cant_sect;
	disco() {}
	disco(int platos, int sectores) : cant_sect(platos*sectores) {}
	long long capacidad(int tam) {return cant_sect*tam;}
};

class bloque
{
public:
	int cant_sect;
	int cant_bloques;
	bloque() {}
	bloque(int sectores,long long sect_totales) : cant_sect(sectores),cant_bloques(sect_totales/cant_sect)
	{
		if (sect_totales%cant_sect > 0) cant_bloques++;
	}
	long long capacidad(int tam) {return cant_sect*tam;}
};

struct disco_virtual
{
	disco DS;
	plato PL;
	pista PS;
	sector SC;
	bloque BL;
};

struct dato
{
	int tipo;
	int tambytes;
};

class reg_eliminado
{
public:
	reg_eliminado* sig;
	int id;
	reg_eliminado(int n) : id(n)
	{
		sig = NULL;
	}
};

class lista_eliminados
{
	reg_eliminado* cabeza;
	int cant_reg_elim;
public:
	lista_eliminados ()
	{
		cabeza = NULL;
		cant_reg_elim = 0;
	}

	void add_reg_elim (int n)
	{
		reg_eliminado* nuevo = new reg_eliminado(n);
		reg_eliminado* temp_before = NULL;
		reg_eliminado* temp_actual = cabeza;

		while(true)
		{
			if (temp_actual == NULL)
			{
				if (!cabeza)
				{
					cabeza = nuevo;
					cant_reg_elim++;
					return;
				}
				temp_before->sig = nuevo;
				return;
			}
			else if (temp_actual->id < n)
			{
				temp_before = temp_actual;
				temp_actual = temp_actual->sig;
			}
			else if (temp_actual->id == n)
			{
				std::cout << "YA HABIA SIDO ELIMINADO!\n";
				delete nuevo;
				return;
			}
			else if (temp_actual->id > n)
			{
				nuevo->sig = temp_actual;
				if (temp_actual == cabeza) cabeza = nuevo;
				else temp_before->sig = nuevo;

				cant_reg_elim++;
				return;
			}
		}
	}

	bool find_elim (int n)
	{
		reg_eliminado* temp = cabeza;

		while (temp)
		{
			if (temp->id == n) return true;
			else temp = temp->sig;
		}
		return false;
	}

	void borrar_elim ()
	{
		if (!cabeza)
		{
			std::cout << "Ningun registro ha sido eliminado antes\n";
			return;
		}

		reg_eliminado* temp = cabeza;
		cabeza = temp->sig;
		cant_reg_elim--;
		delete temp;
	}

	int get_regxllenar ()
	{
		if (!cabeza) return 0;
		return cabeza->id;
	}
};

disco_virtual* creardisco()
{
	std::cout << "\nFASE 2: DISCO\n";
	int cantplatos, cantpistas, cantsectores, tamsector;
	//Todas estas cantidades son de acuerdo a su unidad mayor, ejemplo platos por disco, pistas por superficie de plato, sectores por pistas
	//std::cout << "Cantidad de discos: ";
	//std::cin >> cantdiscos;
	std::cout << "Cantidad de platos: ";
	std::cin >> cantplatos;
	std::cout << "Cantidad de pistas por superficie: ";
	std::cin >> cantpistas;
	std::cout << "Cantidad de sectores por pista: ";
	std::cin >> cantsectores;
	std::cout << "Capacidad de un sector: ";
	std::cin >> tamsector;

	disco_virtual *MIDISCO = new disco_virtual;
	MIDISCO->SC = sector(cantsectores,tamsector);
	MIDISCO->PS = pista(cantpistas,cantsectores);
	MIDISCO->PL = plato(cantplatos,cantpistas,MIDISCO->PS.cant_sect);
	MIDISCO->DS = disco(cantplatos,MIDISCO->PL.cant_sect);

	std::cout << "LA CAPACIDAD DEL DISCO ES... " << MIDISCO[0].DS.capacidad(tamsector) << " bytes\n";
	return MIDISCO;
}

void cargar_datos(int& byte_registro, int& cant_registros, dato* &columnas, int& cant_columnas)
{
	std::cout << "FASE 1: FILE\n";
	int temp;
	char letra;
	char tipo[50];
	int num_tipo;
	int cant_letras;
	ifstream esq("titanic_esquema.txt");

	while (esq >> noskipws >> letra)
	{
		if (letra == ' ')
		{
			cant_letras = 0;
			esq >> noskipws >> letra;
			while (letra != '(')
			{
				tipo[cant_letras++] = letra;
				esq >> noskipws >> letra;
			}
			tipo[cant_letras] = '\0';

			num_tipo = -1;
			if (strcmp(tipo,"varchar") == 0) num_tipo = 0;
			else if (strcmp(tipo,"numeric") == 0) num_tipo = 1;
			else if (strcmp(tipo,"bool") == 0) num_tipo = 2;
			else if (strcmp(tipo,"char") == 0) num_tipo = 3;
			
			std::cout << "EL TIPO ES: " << tipo << endl;
			std::cout << "DE NUMERO: " << num_tipo << endl;

			temp = 0;
			esq >> noskipws >> letra;
			while (letra != ',' && letra != ')')
			{
				temp = 10*temp + ((int)letra-48);
				esq >> noskipws >> letra;
			}
			//if (letra != ',') //En el caso de numeric tendría que obtener un valor mas

			columnas[cant_columnas++] = {num_tipo,temp};
			byte_registro += temp;
		}
	}
	
	std::cout << "#bytes por registro: " << byte_registro << endl;
	std::cout << "#bits por registro: " << byte_registro*8 << endl;
	esq.close();
	
	int bit_file = 0;
	ifstream base("titanic.txt");
	base.seekg(0,ios::end);
	bit_file = base.tellg();
	std::cout << "#bits en el FILE: " << bit_file << endl;
	base.close();

	cant_registros = bit_file/(byte_registro*8);
	std::cout << "#registros: " << cant_registros << endl;
}

void imprimir_registro(const int byte_registro, int& cant_registros)
{
	int n;
	char *r;
	std::cout << "#REGISTRO A IMPRIMIR? ";
	while (true)
	{
		std::cin >> n;
		if (n > cant_registros || n <= 0)
			std::cout << "INGRESE UN VALOR VALIDO!\n";
		else break;
	}

	ifstream base("titanic.txt");
	base.seekg(byte_registro*8*(n-1),ios::beg);
	r = new char[byte_registro*8];
	base.read(r,byte_registro*8);
	base.close();

	for (int i = 0; i < byte_registro*8; i++) std::cout << r[i];
	std::cout << ".\n";
}

/*
void cargar_sectores_variable(disco_virtual *MIDISCO, const int& cant_registros, const int& byte_registro, dato* &columnas, int& cant_columnas)
{
	std::cout << "\nFASE 3: CARGAR SECTORES\n";
	for(int i = 1; i <= MIDISCO[0].DS.cant_sect; i++)
	{
		ofstream f("S["+to_string(i)+"].txt");
		f.close();
	}

	int id_registro = 0;
	char registro_aux[byte_registro*8];
	char registro_base[byte_registro*8];
	for (int i = 0; i < byte_registro*8; i++) registro_aux[i] = ' ';
	int pos = 0;

	ifstream base("titanic.txt");
	while (true)
	{
		base.read(registro_base,byte_registro*8);
		pos = cant_columnas/8;
		if (cant_columnas%8 > 0) pos += 8;

		for(int i = 0; i < cant_columnas; i++) if (columnas[i].tipo == 0) pos+=16;
		
		for(int i = 0; i < cant_columnas; i++)
		{
			if (columnas[i].tipo != 0)
			{
				c 
			}
		}

		for(int j = 0; j < columnas[i].tambytes*8; j++) {}
	}

}
*/

void cargar_sectores(disco_virtual *MIDISCO, const int& cant_registros, const int& byte_registro)
{
	std::cout << "\nFASE 3: CARGAR SECTORES\n";
	for(int i = 1; i <= MIDISCO->DS.cant_sect; i++)
	{
		ofstream f("S["+to_string(i)+"].txt");
		f.close();
	}

	int sec_cargados = 0;
	int registros_sector = MIDISCO->SC.capacidad/byte_registro;

	std::cout << "Cargando sectores...\n";
	std::cout << "Registros por sector: " << registros_sector << endl;
	char letra_carga;

	ifstream base("titanic.txt");
	while(sec_cargados*registros_sector < cant_registros)
	{
		base.seekg(byte_registro*8*registros_sector*sec_cargados,ios::beg);
		//base.read(sec_carga,byte_registro*8*registros_sector);

		ofstream file_sect("S["+to_string(++sec_cargados)+"].txt");
		//file_sect.write(sec_carga,byte_registro*8*registros_sector);
		///*
		for (int r = 0; r < registros_sector; r++)
		{
			for (int l = 0; l < byte_registro*8; l++)
			{
				base >> noskipws >> letra_carga;
				file_sect << letra_carga;
			}
		}
		//*/
		file_sect.close();
	}
	base.close();

	std::cout << "PROCESO TERMINADO!\n";
}

void imprimir_sector(disco_virtual *MIDISCO,const int byte_registro, dato* &columnas, const int& cant_columnas)
{
	int s;
	std::cout << "#SECTOR A IMPRIMIR? ";
	while (true)
	{
		std::cin >> s;
		if (s > MIDISCO->DS.cant_sect || s <= 0)
			std::cout << "INGRESE UN VALOR VALIDO!\n";
		else break;
	}

	char reg[byte_registro*8];
	int atributo;
	int pos;
	ifstream sect("S["+to_string(s)+"].txt");
	while(sect.read(reg,byte_registro*8))
	{
		atributo = 0;
		pos = 0;
		for (int j = 0; j < byte_registro*8; j++)
		{
			if (reg[j] == ' ')
			{
				if (reg[j+1] == ' ') 
				{
					pos += columnas[atributo++].tambytes;
					j = pos*8;
					if (j >= byte_registro*8) break;
					std::cout << " | ";
				}
			}
			std::cout << reg[j];
		}
		std::cout << endl;
	}
	sect.close();

	std::cout << "\nUBICACION DEL SECTOR:\n";
	int d = s/MIDISCO->DS.cant_sect;
	if (s%MIDISCO->DS.cant_sect != 0) d++;
	std::cout << "DISCO: " << d << endl;
	if (s%MIDISCO->DS.cant_sect != 0) s = s%MIDISCO->DS.cant_sect;

	int p = s/MIDISCO->PL.cant_sect;
	if (s%MIDISCO->PL.cant_sect != 0) p++;
	std::cout << "PLATO: " << p << endl;
	if (s%MIDISCO->PL.cant_sect != 0) s = s%MIDISCO->PL.cant_sect;

	std::cout << "SUPERFICIE: ";
	if (MIDISCO->PL.cant_sect/2 < s)
	{
		std::cout << "INFERIOR";
		s -= MIDISCO->PL.cant_sect/2;
	}
	else std::cout << "SUPERIOR";
	std::cout << endl;
	
	int ps = s/MIDISCO->PS.cant_sect;
	if (s%MIDISCO->PS.cant_sect != 0) ps++;
	std::cout << "PISTA: " << ps << endl;
	if (s%MIDISCO->PS.cant_sect != 0) s = s%MIDISCO->PS.cant_sect;
	std::cout << "SECTOR: " << s << endl;
}

void cargar_bloques(disco_virtual *MIDISCO, const int& byte_registro)
{
	int sxb;
	std::cout << "\nFASE 4: BLOQUES\n";
	std::cout << "Sectores por bloque: ";
	std::cin >> sxb;
	MIDISCO->BL = bloque(sxb,MIDISCO->DS.cant_sect);
	std::cout << "Capacidad del bloque: " << MIDISCO->BL.capacidad(MIDISCO->SC.capacidad) << endl;
	std::cout << "Registros por bloque: " << MIDISCO->BL.cant_sect*(MIDISCO->SC.capacidad/byte_registro) << endl;
	std::cout << "Cantidad de bloques: " << MIDISCO->BL.cant_bloques << endl;
}

void imprimir_bloque(disco_virtual *MIDISCO, const int& byte_registro, dato* &columnas, const int& cant_columnas)
{
	int b;
	std::cout << "#BLOQUE A IMPRIMIR? ";
	while (true)
	{
		std::cin >> b;
		if (b > MIDISCO->BL.cant_bloques || b <= 0)
			std::cout << "INGRESE UN VALOR VALIDO!\n";
		else break;
	}

	int s = (b-1)*MIDISCO->BL.cant_sect+1;

	char reg[byte_registro*8];
	int atributo;
	int pos;
	
	std::cout << "Sectores del bloque: " << s << " al ";
	if ((s+MIDISCO->BL.cant_sect)-1 > MIDISCO->DS.cant_sect) std::cout << MIDISCO->DS.cant_sect;
	else std::cout << (s+MIDISCO->BL.cant_sect)-1;
	std::cout << endl;
	
	for (int i = 0; i < MIDISCO->BL.cant_sect; i++)
	{
		ifstream sect("S["+to_string(s+i)+"].txt");
		while(sect.read(reg,byte_registro*8))
		{
			atributo = 0;
			pos = 0;
			for (int j = 0; j < byte_registro*8; j++)
			{
				if (reg[j] == ' ')
				{
					if (reg[j+1] == ' ') 
					{
						pos += columnas[atributo++].tambytes;
						j = pos*8;
						if (j >= byte_registro*8) break;
						std::cout << " | ";
					}
				}
				std::cout << reg[j];
			}
			std::cout << endl;
		}
		/*
		while(sect >> noskipws >> letra)
		{
			sect.seekg(byte_registro*8*registros+posenreg,ios::beg);
			while (atributo == 0)
			{
				if (letra == ' ')
				{
					sect >> noskipws >> letra;
					if (letra == ' ') atributo++;
					else std::cout << ' ';
				}
				std::cout << letra;
			}
			

			if (letra == ' ')
			{
				sect >> noskipws >> letra;
				if (letra == ' ')
				{
					while (letra == ' ') sect >> noskipws >> letra;
					if (((int)sect.tellg()-1)%1080 == 0) std::cout << endl;
					else std::cout << " | ";
				}
				else std::cout << ' ';
			}
			std::cout << letra;
		}
		*/
		sect.close();
	}
}

void eliminar_registro(disco_virtual *MIDISCO, const int& cant_registros, const int& byte_registro, lista_eliminados& LE)
{
	int r;
	std::cout << "Registro a eliminar: ";
	while (true)
	{
		std::cin >> r;
		if (r > cant_registros || r <= 0)
			std::cout << "INGRESE UN VALOR VALIDO!\n";
		else break;
	}

	if (LE.find_elim(r))
	{
		std::cout << "El registro ya habia sido eliminado!";
		return;
	}

	int reg_sector = MIDISCO->SC.capacidad/byte_registro;
	int s = r/reg_sector;
	if (r%reg_sector > 0) {s++;}

	bool confir;
	while (true)
	{
		std::cout << "Esta seguro de eliminar el registro? Esta accion es irrevertible (SI = 1, NO = 0): ";
		std::cin >> confir;
		if (confir) break;
		else if (!confir) std::cout << "INGRESE UN VALOR VALIDO!\n";
	}

	fstream sec_elim("S["+to_string(s)+"].txt");
	if (r%reg_sector == 0) sec_elim.seekp((reg_sector-1)*byte_registro*8,ios::beg);
	else sec_elim.seekp(((r%reg_sector)-1)*8,ios::beg);
   	for (int i = 0; i < byte_registro*8; i++) sec_elim << ' ';
	sec_elim.close();

	LE.add_reg_elim(r);
}

void intercambiar_registro(disco_virtual *MIDISCO, const int& cant_registros, const int& byte_registro)
{
	int r1;
	int r2;

	std::cout << "Registros a reemplazar\n";
	
	while (true)
	{
		std::cout << "R1: ";
		std::cin >> r1;
		if (r1 > cant_registros || r1 <= 0)
			std::cout << "INGRESE UN VALOR VALIDO!\n";
		else break;
	}

	while (true)
	{
		std::cout << "R2: ";
		std::cin >> r2;
		if (r2 > cant_registros || r2 <= 0)
			std::cout << "INGRESE UN VALOR VALIDO!\n";
		else break;
	}

	char cont_reg1[byte_registro*8];
	char cont_reg2[byte_registro*8];

	int reg_sector = MIDISCO[0].SC.capacidad/byte_registro;
	int s1 = r1/reg_sector;
	if (r1%reg_sector > 0) {s1++;}
	int s2 = r2/reg_sector;
	if (r2%reg_sector > 0) {s2++;}
	int pos1;
	int pos2;

	fstream sec1_reg("S["+to_string(s1)+"].txt");
	fstream sec2_reg("S["+to_string(s2)+"].txt");

	if (r1%reg_sector == 0) pos1 = (reg_sector-1)*byte_registro*8;
	else pos1 = ((r1%reg_sector)-1)*8;
	sec1_reg.seekg(pos1,ios::beg);
	sec1_reg.read(cont_reg1,byte_registro*8);

	if (r2%reg_sector == 0) pos2 = (reg_sector-1)*byte_registro*8;
	else pos2 = ((r2%reg_sector)-1)*8;
	sec2_reg.seekg(pos2,ios::beg);
	sec2_reg.read(cont_reg2,byte_registro*8);

	sec1_reg.seekp(pos1,ios::beg);
	sec1_reg.write(cont_reg2,byte_registro*8);
	sec2_reg.seekp(pos2,ios::beg);
	sec2_reg.write(cont_reg1,byte_registro*8);

	sec1_reg.close();
	sec2_reg.close();
}

void adicionar_registro(disco_virtual *MIDISCO, const int& cant_registros, const int& byte_registro, lista_eliminados& LE)
{
	int n = LE.get_regxllenar();

	if (n == 0) n = cant_registros+1;
	int reg_sector = MIDISCO->SC.capacidad/byte_registro;
	int s = n/reg_sector;
	if (n%reg_sector > 0) {s++;}

	char reg[byte_registro*8];
	fstream sec_add("S["+to_string(s)+"].txt");
	sec_add.seekp(((n%reg_sector)-1)*8,ios::beg);
	//
	//
	//
	sec_add.close();
}

int main()
{
	int llave = 1;
	int byte_registro = 0;
	int cant_registros = 0;
	dato *columnas = new dato[50];
	int cant_columnas = 0;
	cargar_datos(byte_registro,cant_registros,columnas,cant_columnas);
	//imprimir_registro(byte_registro,cant_registros);
	
	/*
	while(llave == 1)
	{
		imprimir_registro(byte_registro,cant_registros);
		std::cout << "Desea imprimir otro registro? (SI = 1,NO = 0): ";
		std::cin >> llave;
	}
	llave = 1;
	*/
	
	disco_virtual* MIDISCO = creardisco();
	cargar_sectores(MIDISCO,cant_registros,byte_registro);

	/*
	while(llave == 1)
	{
		imprimir_sector(MIDISCO,byte_registro,columnas,cant_columnas);
		std::cout << "Desea imprimir otro sector? (SI = 1,NO = 0): ";
		std::cin >> llave;
	}
	llave = 1;
	*/

	cargar_bloques(MIDISCO,byte_registro);

	while(llave == 1)
	{
		imprimir_bloque(MIDISCO,byte_registro,columnas,cant_columnas);
		std::cout << "Desea imprimir otro bloque? (SI = 1,NO = 0): ";
		std::cin >> llave;
	}
	llave = 1;
	
	return 0;
}