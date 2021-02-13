open Utils;

type error;
type unified;

type position = {
  column: int,
  line: int,
};

type positionAst = {
  [@bs.as "end"]
  end_: position,
  start: position,
};

type sectionAst = {
  actionable: bool,
  children: array(sectionAst),
  content: Js.nullable(string),
  level: int,
  position: positionAst,
  tags: Js.nullable(array(string)),
  [@bs.as "type"]
  type_: string,
};

type propertiesAst = {
  date: string,
  template: string,
  title: string,
};

type orgAst = {
  children: array(sectionAst),
  properties: propertiesAst,
};

type orgItem =
  | Unmatched
  | Section({
      children: array(sectionAst),
      level: int,
      position: positionAst,
    })
  | Headline({
      children: array(sectionAst),
      content: string,
      level: int,
      position: positionAst,
      tags: array(string),
    });

let getItem = item =>
  switch (item.type_) {
  | "section" =>
    Section({
      children: item.children,
      level: item.level,
      position: item.position,
    })
  | "headline" =>
    Headline({
      children: item.children,
      content: nullableOrEmptyStr(item.content),
      level: item.level,
      position: item.position,
      tags: nullableOrEmptyArray(item.tags),
    })
  | _ => Unmatched
  };

let getMainItem = item =>
  switch (getItem(item)) {
  | Section({children}) => children
  | _ => [||]
  };

module Org = {
  [@bs.module "orga"] external parseOrga: string => orgAst = "parse";
};
