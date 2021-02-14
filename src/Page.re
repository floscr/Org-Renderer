open Orga;
open ReactUtils;

let orga =
  Org.parseOrga(
    {j|* TODO Headline :FOO:FAA:
** Child 1

Content

- Foo
  + Bar
  + Bar
- Bar
- Baz

1. Foo
2. Bar
3. Baz

*** Child2

|j},
  );

module Heading = {
  [@react.component]
  let make = (~level: int, ~children) => {
    switch (level) {
    | 1 => <h1> children </h1>
    | 2 => <h2> children </h2>
    | 3 => <h3> children </h3>
    | 4 => <h4> children </h4>
    | 5 => <h5> children </h5>
    | _ => children
    };
  };
};

let wrapWithKey = (level, index, children) => {
  let key = {j|$level-$index|j};
  <React.Fragment key> children </React.Fragment>;
};

let renderTags = xs =>
  <ul>
    {Belt.Array.mapWithIndex(
       xs,
       (i, x) => {
         let key = string_of_int(i);
         <li key> {s(x)} </li>;
       },
     )
     |> React.array}
  </ul>;

let renderHeadline = x =>
  switch (x) {
  | Headline({content, level, tags}) =>
    <header>
      <Heading level> {s(content)} </Heading>
      {renderTags(tags)}
    </header>
  | _ => React.null
  };

let renderParagraphs = xs => {
  Belt.Array.mapWithIndex(xs, (i, x) => {
    switch (getItem(x)) {
    | PlainText({value}) => <p key={string_of_int(i)}> {s(value)} </p>
    | _ => React.null
    }
  })
  |> React.array;
};

let rec renderList = (xs, ordered) => {
  Belt.Array.mapWithIndex(
    xs,
    (i, x) => {
      let key = string_of_int(i);
      switch (getItem(x)) {
      | List({children, ordered}) =>
        <li key> {renderList(children, ordered)} </li>
      | ListItem({children}) => <li key> {renderParagraphs(children)} </li>
      | _ => React.null
      };
    },
  )
  |> React.array
  |> (
    xs =>
      switch (ordered) {
      | true => <ol> xs </ol>
      | _ => <ul> xs </ul>
      }
  );
};

let rec renderItems = xs => {
  Belt.Array.mapWithIndex(xs, (i, x) => {
    switch (getItem(x)) {
    | Headline({level}) as z => renderHeadline(z) |> wrapWithKey(level, i)
    | Section({children, level}) =>
      renderItems(children) |> wrapWithKey(level, i)
    | Paragraph({children}) => renderParagraphs(children)
    | List({children, ordered}) => renderList(children, ordered)
    | _ => React.null
    }
  })
  |> React.array;
};

let render = () =>
  orga.children[0] |> Orga.getMainItem |> Utils.log |> renderItems;

[@react.component]
let make = () => <> {render()} </>;
