// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/ConsoleGUI.h"
#include "stdafx.h"
#include "Utils/Text/String.h"
#include "Settings/Settings.h"


ConsoleGUI *ConsoleGUI::self = nullptr;


ConsoleGUI::ConsoleGUI(wxFrame *parent) : wxFrame(parent, wxID_ANY, "Устройство программирования карт NTAG213")
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &ConsoleGUI::OnSize, this);
    line->Bind(wxEVT_TEXT_ENTER, &ConsoleGUI::OnEnterKey, this, ID_LINE);
    line->Bind(wxEVT_KEY_DOWN, &ConsoleGUI::OnTextControlKeyDown, this, ID_LINE);
    Bind(wxEVT_CLOSE_WINDOW, &ConsoleGUI::OnClose, this);
}

ConsoleGUI::~ConsoleGUI()
{
    self = nullptr;
}


void ConsoleGUI::OnSize(wxSizeEvent &)
{
    wxPoint clientOrigin = GetClientAreaOrigin();

    wxSize clientSize = GetClientSize();

    int heightLine = line->GetSize().y;

    wxSize sizeText = clientSize;
    sizeText.y -= heightLine;

    text->SetSize(sizeText);

    line->SetPosition({ clientOrigin.x, clientSize.y - heightLine });
    line->SetSize({ text->GetSize().x, heightLine });
}


void ConsoleGUI::Create()
{
    if (!self)
    {
        self = new ConsoleGUI(nullptr);

        self->SetPosition({ 0, 0 });

        self->SetClientSize({ 300, 200 });

        self->Maximize(false);

        self->Show(false);
    }
}


void ConsoleGUI::OnEnterKey(wxCommandEvent &)
{
    String<> command(line->GetLineText(0).c_str());

    history.Add(command.c_str());

    if (command[0] == '#')
    {
//        Reader::Send(command.c_str());
    }
    else
    {
//        String<> first_word = command.GetWord(1, " \0");
//        first_word.ToUpper();
//
//        if (first_word == "HELP")
//        {
//            AddLine("Команды:");
//            AddLine("\tCtrl-K - открыть/закрыть консоль");
//            AddLine("\tclear - очистить консоль");
//            //AddLine("\tpages - показать дополнительные вкладки");
//        }
//        else
//        {
//            if (first_word == "CLEAR")
//            {
//                text->Clear();
//            }
//        }
    }

    line->Clear();
}


void ConsoleGUI::OnTextControlKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_UP)
    {
        wxString txt = history.Prev();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else if (event.GetKeyCode() == WXK_DOWN)
    {
        wxString txt = history.Next();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else
    {
        event.Skip();
    }
}


void ConsoleGUI::AddLine(const wxString &str)
{
    mutex.lock();

    text->WriteText(str);
    text->WriteText(wxT("\n"));

    mutex.unlock();
}


void ConsoleGUI::SwitchVisibility()
{
    self->Show(!self->IsShown());
}


void ConsoleGUI::OnClose(wxCloseEvent &)
{
    self->Show(false);
}


void ConsoleGUI::History::Add(const wxString &txt)
{
    if ((history.size() == 0) || 
        (history[history.size() - 1].compare(txt) != 0))
    {
        history.push_back(txt);
        position = history.size() - 1;
    }
}


wxString ConsoleGUI::History::Next()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position++;
    if (position == history.size())
    {
        position = 0;
    }

    return result;
}


wxString ConsoleGUI::History::Prev()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position = (position == 0) ? (history.size() - 1) : (position - 1);

    return result;
}
