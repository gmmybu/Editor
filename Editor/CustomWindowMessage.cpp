#include "stdafx.h"
#include "CustomWindowMessage.h"

UINT WM_CUSTOM_AFTER_CREATE = 
	RegisterWindowMessage("WM_CUSTOM_AFTER_CREATE");

UINT WM_CUSTOM_AFTER_TREE_ITEM_SELECTED = 
	RegisterWindowMessage("WM_CUSTOM_AFTER_TREE_ITEM_SELECTED");
