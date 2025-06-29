##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Utils
##

def isNumber(str: str):
    try:
        int(str)
        return True
    except:
        return False
