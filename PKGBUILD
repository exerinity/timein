# Maintainer: exerinity <me@exerinity.com>
pkgname=timein-git
pkgver=1
pkgrel=1
pkgdesc="A minimal tool for quickly getting the time and date in another city or timezone"
arch=('x86_64')
url="https://github.com/exerinity/timein"
license=('MIT')
depends=('glibc')
makedepends=('git')
source=("$pkgname-$pkgver.tar.gz::https://github.com/exerinity/$pkgname/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('SKIP')

pkgver() {
    cd "$pkgname"
    git rev-list --count HEAD
}

build() {
    cd "$pkgname"
    gcc -O2 -o timein execute.c find.c listofcities.c
}

package() {
    cd "$pkgname"
    install -Dm755 timein "$pkgdir/usr/bin/timein"
}