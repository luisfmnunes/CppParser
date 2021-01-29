#include "../include/parser_module.h"

//Returns an error message of a dcppError
std::string deCiPPher::error_message (dcppError error, std::string var=""){
            switch (error)
            {
            case dcppError::dcpp_OK:
                return "OK Status";
                break;
            case dcppError::dcpp_ALREADY_SET:
                return "Directive already set and binded to variable";
                break;
            case dcppError::dcpp_WRONG_TYPE:
                return "Value passed to numeric variable is not numeric";
                break;
            case dcppError::dcpp_NOT_EXIST:
                return "Directive passed isn't binded to a variable";
                break;
            case dcppError::dcpp_DOUBLE_BIND:
                return "Variable is already binded to another directive";
                break;
            case dcppError::dcpp_REQUIRED_NOT_SET:
                return "Variable " + var + (var.empty() ? "" : " ") + "set as required isn't initialized";
                break;

            default:
                break;
            }
        }