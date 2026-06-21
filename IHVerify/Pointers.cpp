#include "IHVerify.h"
#include "Pointers.h"

// PointerExpiredProcess is implemented in IHVerify.Marker.cpp
// This file exists to:
// 1) satisfy the linker with the declaration from Pointers.h
// 2) provide a place for IHVerify to expand pointer-lifetime management
//
// DO NOT call GetClassName/vtable on pDyingObj — it has already been freed.
// pDyingObj may be broadcast multiple times (multiple references holding it).
// bRemoved=false means the object is temporarily leaving the map, not dying.
