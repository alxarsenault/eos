#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axLabel.h"

#include "eosFrame.h"

enum CalcOperator
{
    CALC_ADD, CALC_MIN, CALC_MUL, CALC_DIV, CALC_POW, CALC_MODULO,
    CALC_SIGN, CALC_NUMBER, CALC_EQUAL, CALC_RESET
};

class CalculatorLogic
{
public:
    CalculatorLogic() : _value_left(0), _result(0), _string_value("0"),
    _operator(CALC_EQUAL)
    {
    }
    
    void SetOperation(const CalcOperator& operation)
    {
        switch (operation)
        {
            case CALC_RESET:
                _result = _value_left = 0;
                _string_value = "0";
                _operator = CALC_EQUAL;
                break;
            case CALC_SIGN:
                if (_string_value[0] == '-')
                    _string_value.erase(_string_value.begin());
                else _string_value.insert(0, "-"); break;
                
            case CALC_EQUAL: Equal(); break;
            default: TwoSideOperation(operation);
        }
    }
    
    double GetValue()
    {
        return stod(_string_value);
    }
    
    void AddNumber(const std::string& number)
    {
        if (number == ".")
        {
            // If not found.
            if (_string_value.find('.') == std::string::npos)
                _string_value += number;
        }
        else
            _string_value += number;
    }
    
private:
	std::string _string_value;
    double _value_left, _result;
    CalcOperator _operator;
    
    void TwoSideOperation(const CalcOperator& operation)
    {
        if (_operator != CALC_EQUAL) Equal();
        _value_left = stod(_string_value);
        _operator = operation;
        _string_value = "0";
    }
    
    void Equal()
    {
        double v = stod(_string_value);
        
        switch (_operator)
        {
            case CALC_RESET: _result = 0; break;
            case CALC_ADD: _result = _value_left + v; break;
            case CALC_MIN: _result = _value_left - v; break;
            case CALC_MUL: _result = _value_left * v; break;
            case CALC_POW: _result = pow(_value_left, v); break;
            case CALC_MODULO: _result = int(floor(_value_left)) % int(floor(v)); break;
            case CALC_DIV: v == 0 ? _result = 0 : _result = _value_left / v; break;
        }
        
        _operator = CALC_EQUAL;
        _string_value = std::to_string(_result);
    }
};

class Calculator: public ax::Window::Backbone
{
public:
    Calculator(const ax::Rect& rect)
    {
		win = ax::Window::Create(rect);
		
        AddEventFunction("OnNumber", GetOnNumber());
        AddEventFunction("OnOperation", GetOnOperation());
        axObjectLoader loader(this, "resource/Calculator.xml");

		std::vector<std::pair<std::string, ax::StringPairVector>> obj_data = loader.GetObjData();
	
		CreateObject(obj_data);
    }

    
    axEVENT_ACCESSOR(ax::Button::Msg, OnNumber);
    axEVENT_ACCESSOR(ax::Button::Msg, OnOperation);
    
private:
    CalculatorLogic _logic;
    
	void CreateObject(const std::vector<std::pair<std::string, ax::StringPairVector>>& obj_data)
	{
		for(auto& n : obj_data) {
			if(n.first == "axButton") {
				ax::Button::Builder builder(this);
				builder.Create(n.second);
			}
			else if(n.first == "axLabel") {
				axLabel::Builder builder(this);
				builder.Create(n.second);
			}
		}
	}	

    // Events
    void OnNumber(const ax::Button::Msg& msg)
    {
        _logic.AddNumber(msg.GetMsg());
        axLabel* screen = GetResourceManager()->GetResource("screen");

		ax::Print("Value :", _logic.GetValue());
        screen->SetLabel(std::to_string(_logic.GetValue()));
    }
    
    void OnOperation(const ax::Button::Msg& msg)
    {
        _logic.SetOperation((CalcOperator)stoi(msg.GetMsg()));
        axLabel* screen = GetResourceManager()->GetResource("screen");
        screen->SetLabel(std::to_string(_logic.GetValue()));
    }
    
    void OnPaint(ax::GC gc)
    {
		ax::Rect rect(win->dimension.GetDrawingRect());
        
		gc.SetColor(ax::Color(0.4));
		gc.DrawRectangle(rect);
        //gc.DrawRectangleColorFade(rect, ax::Color(0.7), ax::Color(0.8));
        gc.SetColor(ax::Color(0.5));
        gc.DrawRectangleContour(rect);
    }
};


extern "C"
{
	axWindow* StartApplication(axWindow* parent, ax::Rect rect)
	{
		ax::Print("Start calculator");
		eos::Frame* frame = new eos::Frame(parent, rect, "Calculator");
		//frame->AddProperty("Resizable");

		Calculator* calc = new Calculator(frame, frame->GetChildRect());
		frame->SetChildHandler(calc);
		
		return frame;
	}
}

//int main(int argc, char* argv[])
//{
//    axApp* app = axApp::CreateApp(axSize(165, 255));
//    
//    app->AddMainEntry([]()
//                      {
//                          new Calculator(nullptr, axRect(0, 0, 165, 255));
//                      });
//    
//    app->MainLoop();
//    return 0;
//}
