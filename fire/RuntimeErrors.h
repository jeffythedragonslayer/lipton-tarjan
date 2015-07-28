#pragma once
#include "stdafx.h"

struct ResourceNotExist       : std::runtime_error {ResourceNotExist(std::string const& str)       : runtime_error("ResourceNotExist\n" + str) {} };
struct Zerodet                : std::runtime_error {Zerodet         (std::string const& str)       : runtime_error("Zerodet\n"          + str) {} };
struct ZeroVector             : std::runtime_error {ZeroVector      (std::string const& str)       : runtime_error("ZeroVector\n"       + str) {} };
