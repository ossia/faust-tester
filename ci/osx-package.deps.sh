#!/bin/bash -eux

set +e

export HOMEBREW_NO_AUTO_UPDATE=1
brew install gnu-tar ninja

SDK_ARCHIVE=sdk-macOS.tar.gz
wget -nv https://github.com/ossia/score-sdk/releases/download/sdk26/$SDK_ARCHIVE -O $SDK_ARCHIVE
sudo mkdir -p /opt/ossia-sdk-x86_64/
sudo chown -R $(whoami) /opt
sudo chmod -R a+rwx /opt
gtar xhaf $SDK_ARCHIVE --strip-components=2 --directory /opt/ossia-sdk-x86_64/
ls /opt/ossia-sdk-x86_64/

set -e
