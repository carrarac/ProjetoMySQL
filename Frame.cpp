#include "Frame.h"

Frame::Frame(std::string title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	db = new MySQLDatabase("localhost", "seu_usuario", "sua_senha", "nomebanco", 3306);
	if (db) 
		wxMessageBox("Conexão com Banco OK");
	else
		wxMessageBox("Erro ao conectar ao BD");
	
	pnl = new wxPanel(this, wxID_ANY);

	gridCliente = new wxGrid(pnl, wxID_ANY, wxPoint(200, 170), wxSize(-1, -1));
	gridCliente->CreateGrid(0, 0);
	
	new wxStaticText(pnl, wxID_ANY, "Nome cliente", wxPoint(20, 20), wxSize(-1, -1));
	cliCampoNome = new wxTextCtrl(pnl, wxID_ANY, "", wxPoint(20, 40), wxSize(200, -1));
	new wxStaticText(pnl, wxID_ANY, "Email cliente", wxPoint(20, 70), wxSize(-1, -1));
	cliCampoEmail = new wxTextCtrl(pnl, wxID_ANY, "", wxPoint(20, 90), wxSize(200, -1));
	btnInserir = new wxButton(pnl, wxID_ANY, "Inserir", wxPoint(240, 90), wxSize(-1, -1));
	btnConsultar = new wxButton(pnl, wxID_ANY, "Consulta", wxPoint(320, 90), wxSize(-1, -1));
	btnExcluir = new wxButton(pnl, wxID_ANY, "Excluir selecionado", wxPoint(400, 90), wxSize(-1, -1));

	btnInserir->Bind(wxEVT_BUTTON, &Frame::on_btnInserir_clicked, this);
	btnConsultar->Bind(wxEVT_BUTTON, &Frame::on_btnConsultar_clicked, this);
	btnExcluir->Bind(wxEVT_BUTTON, &Frame::on_btnExcluir_clicked, this);
}
Frame::~Frame() {
	delete db;
}

void Frame::carregaDados(std::string sql) {
	auto matriz = db->query(sql);
	if (matriz.empty()) return;

	if (gridCliente->GetNumberRows() > 0)
		gridCliente->DeleteRows(0, gridCliente->GetNumberRows());
	
	if (gridCliente->GetNumberCols() > 0)
		gridCliente->DeleteCols(0, gridCliente->GetNumberCols());

	int linhas = matriz.size();
	int colunas = matriz[0].size();
	gridCliente->AppendCols(colunas);
	gridCliente->AppendRows(linhas);

	for(int i = 0; i < linhas; i++)
	{
		for (int j = 0; j < colunas; j++)
		{
			gridCliente->SetCellValue(i, j, matriz[i][j]);
			if (j == 0) gridCliente->SetReadOnly(i, j);
		}
	}
	gridCliente->AutoSize();
	gridCliente->ForceRefresh();
}

void Frame::carregaDados() {
	auto matriz = db->query("SELECT * FROM clientes");
	if (matriz.empty()) return;

	if (gridCliente->GetNumberRows() > 0)
		gridCliente->DeleteRows(0, gridCliente->GetNumberRows());

	if (gridCliente->GetNumberCols() > 0)
		gridCliente->DeleteCols(0, gridCliente->GetNumberCols());

	int linhas = matriz.size();
	int colunas = matriz[0].size();
	gridCliente->AppendCols(colunas);
	gridCliente->AppendRows(linhas);

	for (int i = 0; i < linhas; i++)
	{
		for (int j = 0; j < colunas; j++)
		{
			gridCliente->SetCellValue(i, j, matriz[i][j]);
			if (j == 0) gridCliente->SetReadOnly(i, j);
		}
	}
	gridCliente->AutoSize();
	gridCliente->ForceRefresh();
}

void Frame::on_btnInserir_clicked(wxCommandEvent& evt)
{
	if(!cliCampoNome->GetValue().IsEmpty())
	{
		std::string nome = cliCampoNome->GetValue().ToStdString();
		std::string email = cliCampoEmail->GetValue().ToStdString();

		std::string sql = "INSERT INTO clientes (nome, email) VALUES ('" +
			db->escape(nome) + "', '" +
			db->escape(email) + "')";
		if (db->execute(sql)) {
			this->SetStatusText(wxString::Format("Último ID de cliente inserido %llu.", db->getLastId()));
			cliCampoNome->Clear();
			cliCampoEmail->Clear();
		}
	}
	else
	{
		wxMessageBox("É necessário incluir nome", "Alerta", wxICON_WARNING);
	}
}

void Frame::on_btnConsultar_clicked(wxCommandEvent& evt)
{
	std::string nome = cliCampoNome->GetValue().ToStdString(),
		email = cliCampoEmail->GetValue().ToStdString();

	std::string sql = "SELECT * FROM clientes cli WHERE cli.nome like '%"
		+ db->escape(nome) + "%' and cli.email like '%" + db->escape(email) + "%'";
	if (db->execute(sql))
	{
		carregaDados(sql);
	}
}

void Frame::on_btnExcluir_clicked(wxCommandEvent& evt)
{
	int row = gridCliente->GetGridCursorRow();
	if (row < 0) {
		wxMessageBox("Nenhuma linha selecionada no grid para exclusão", "Aviso", wxICON_WARNING);
		return;
	}
	wxString idStr = gridCliente->GetCellValue(row, 0);
	wxString nome = gridCliente->GetCellValue(row, 1);
	wxMessageDialog dial(this, "Deseja realmente excluir " + nome + "?", "EXCLUSÃO", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if (dial.ShowModal() == wxID_YES) 
	{
		std::string sql = "DELETE FROM clientes WHERE id = " + idStr.ToStdString();
		if (db->execute(sql))
		{
			wxMessageBox("Cliente excluído", "Excluído");
			carregaDados();
		}
	}
		
}