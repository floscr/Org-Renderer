{ stdenv, fetchFromGitHub, ocamlPackages }:

with ocamlPackages;

buildDunePackage {
  pname = "ppx_rapper";
  version = "1.1.0-dev";

  src = fetchFromGitHub {
    owner = "roddyyaga";
    repo = "ppx_rapper";
    rev = "1.1.1";
    sha256 = "0sl2arwifvmijdab2j0bz1s2c332ac3mp6yfnajbqz4blws9wmmj";
  };

  useDune2 = true;

  propagatedBuildInputs = with ocamlPackages; [
    caqti
    caqti-lwt
    base
    pg_query
  ];
}
