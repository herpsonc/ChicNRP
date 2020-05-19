#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


int main(){
	ifstream file;
	ofstream res;
	// A RECUPERER DE L'INSTANCE
	string mois = "mai 2020";
	string line = " ";

	file.open("ResGHR.txt");
	res.open("Solution.xlsx");

	res << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	res << "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\nxmlns:o=\"urn:schemas-microsoft-com:office:office\"\nxmlns:x=\"urn:schemas-microsoft-com:office:excel\"\nxmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\nxmlns:html=\"http://www.w3.org/TR/REC-html40\">";
	res << "<Styles>";
	// Style pour repos : case blanche
	res << "<Style ss:ID=\"Default\" ss:Name=\"Repos\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#FFFFFF\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour w-e : case rose pale
	res << "<Style ss:ID=\"we\" ss:Name=\"Week-End\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#F7D9F7\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour CA : case gris foncé
	res << "<Style ss:ID=\"ca\" ss:Name=\"Congé\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#888888\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour les dates+jours : case rouge
	res << "<Style ss:ID=\"setup\" ss:Name=\"Setup\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\" ss:Bold=\"1\"/>\n<Interior ss:Color=\"#FF0000\" ss:Pattern=\"Solid\"/>\n</Style>";
	// Style pour les jours travaillés : case gris clair
	res << "<Style ss:ID=\"work\" ss:Name=\"Travail\">\n<Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n<Borders>\n<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n</Borders>\n<Font ss:FontName=\"Calibri\" x:Family=\"Swiss\" ss:Size=\"11\" ss:Color=\"#000000\"/>\n<Interior ss:Color=\"#E0E0E0\" ss:Pattern=\"Solid\"/>\n</Style>";
	res << "</Styles>";
	res << "<Worksheet ss:Name =\"" << mois << "\">";
	res << "<Table>";
	
	size_t test, test2;
	do{
		getline(file, line);
		test = line.find("-----");
	} while (test == string::npos);

	getline(file, line);
	
	res << "<Row>";
	res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">Dates</Data></Cell>";
	// RECUPERER NB JOURS DU MOIS
	for(int i=1; i<=31; i++){
			res << "<Cell ss:StyleID=\"setup\"><Data ss:Type=\"Number\">" << i << "</Data></Cell>";
	}
	res << "</Row>";

	line.erase(0, line.find_first_of(':')+1);

	res << "<Row>";
	res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">Jours</Data></Cell>";
	test = line.find_first_of("MTWFS");
	while(test != string::npos){
		res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">" << line[test];
		res << "</Data></Cell>";
		line.erase(0, test+1);
		test = line.find_first_of("MTWFS");
	} 
	res << "</Row>";

	string line2;
	// RECUPERER LE NOMBRE D'AGENTS
	int nb_agents = 8;
	for(int i=0; i<nb_agents; i++){
		getline(file, line);
		res << "<Row>";
		test = line.find_first_of(":");
		res << "<Cell ss:StyleID=\"Default\"><Data ss:Type=\"String\">" << line.substr(0, test) <<"</Data></Cell>";
		line.erase(0, test+1);
		while(!line.empty()){
			test = line.find_first_of("ACDEFJMNORUabcov+");
			line.erase(0, test);
			test2 = line.find_first_of(" ");
			line2 = line.substr(0,test2);
			if(strcmp(line2.c_str(), "Repos") == 0)
					res << "<Cell ss:StyleID=\"Default\"/>";
			else{
				if(strcmp(line2.c_str(), "Ca") == 0)
					res << "<Cell ss:StyleID=\"ca\"/>";
				else{
					res << "<Cell ss:StyleID=\"work\"><Data ss:Type=\"String\">" << line2;
					res << "</Data></Cell>";
				}
			}
			line.erase(0, test2);
		}
		res << "</Row>";
	}

	res << "</Table>";
	res << "</Worksheet>";
	res << "</Workbook>";

	file.close();
	res.close();

}