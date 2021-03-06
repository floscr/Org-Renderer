open Relude.Globals;
open ReOrga;
open OrgClockUtil;

let checkFile = file => {
  let now = Js.Date.make();
  let f = Node.Fs.readFileAsUtf8Sync(file);
  let org = Org.parseOrga(f, Org.defaultOptions);

  filterScheduled(org.children)
  |> filterUpcoming(now)
  |> List.forEach((({children}: sectionAst, _)) => {
       children
       |> Array.head
       |> Option.map(getItem)
       |> Option.tap(
            fun
            | Headline({content}) => {
                Node.Child_process.execSync(
                  {j| notify-send "$content" |j},
                  Node.Child_process.option(),
                )
                |> ignore;
              }
            | _ => (),
          )
       |> ignore
     });
};

checkFile("/home/floscr/Documents/Org/Main/inbox.org");
checkFile("/home/floscr/Documents/Org/Work/work.org");
