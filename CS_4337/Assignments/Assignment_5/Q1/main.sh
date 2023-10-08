#!/bin/bash
# Dynamic Scope Example

x=420

function first()
{
  echo $x
}

function second()
{
  x=69
  first
}

second