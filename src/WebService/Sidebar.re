open FilesTypes;
open ReactUtils;
open Relude.Globals;
open ReactUtils;

module Styles = {
  open Css;
};

type mode =
  | FileBrowser
  | Outline(State.pageState);

type state = {mode};

let initialState = {mode: FileBrowser};

type action =
  | SwitchMode(mode);

let reducer =
    (state: state, action: action): ReludeReact.Reducer.update(action, state) =>
  switch (action) {
  | SwitchMode(mode) => Update({...state, mode})
  | _ => NoUpdate
  };

[@react.component]
let make = (~page) => {
  let (state, send) = ReludeReact.Reducer.useReducer(reducer, initialState);

  let onFileClick = () => send(SwitchMode(Outline(page)));
  let onBackClick = _ => send(SwitchMode(FileBrowser));

  switch (state.mode) {
  | Outline(state) =>
    switch (state) {
    | State.FetchedPage({ast}) =>
      <>
        <button onClick=onBackClick> {"Go back" |> s} </button>
        <Outline ast />
      </>
    | _ => <Files onFileClick={() => ()} />
    }
  | _ => <Files onFileClick />
  };
};
