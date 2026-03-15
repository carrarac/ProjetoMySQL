#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include "MySQLDatabase.h"
class Frame :
    public wxFrame
{
public:
    Frame(std::string title);
    ~Frame();

    void on_btnInserir_clicked(wxCommandEvent& evt);
    void on_btnConsultar_clicked(wxCommandEvent& evt);
    void on_btnExcluir_clicked(wxCommandEvent& evt);
    void carregaDados(std::string sql);
    void carregaDados();
private:
    MySQLDatabase* db;
    wxPanel* pnl;
    wxButton* btnInserir, *btnConsultar, *btnExcluir;
    wxTextCtrl* cliCampoNome, * cliCampoEmail;
    wxGrid* gridCliente;
};

