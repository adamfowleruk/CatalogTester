#!/bin/sh

(head -n 1 en.catkeys && tail -n +2 en.catkeys |sort) > ~/Projects/CatalogTester/en.txt

