#!/bin/sh

kill -9 `ps -A | grep desmume-cli | awk '{print $1}'`
