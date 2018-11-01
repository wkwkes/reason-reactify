/** Simple test cases */
open TestReconciler;
open TestUtility;

/* Use our Reconciler to create our own instance */
module TestReact = Reactify.Make(TestReconciler);

let createRootNode = () => {children: ref([]), nodeId: 0, nodeType: Root};

let aComponent = (~testVal, ~children, ()) =>
  TestReact.primitiveComponent(A(testVal), ~children);
let bComponent = (~children, ()) =>
  TestReact.primitiveComponent(B, ~children);
let cComponent = (~children, ()) =>
  TestReact.primitiveComponent(C, ~children);

let componentWrappingB = (~children, ()) => TestReact.component(() => {
        <bComponent />
}, ~children);

test("Rendering simple wrapped component", () => {
  let rootNode = createRootNode();
  let container = TestReact.createContainer(rootNode);

  let expectedStructure: tree(primitives) =
    TreeNode(
      Root,
      [TreeLeaf(B)],
    );

  TestReact.updateContainer(container, <componentWrappingB />);

  validateStructure(rootNode, expectedStructure);
});

 let componentWrappingAWithProps = (~children, ~v, ()) => TestReact.component(() => <aComponent testVal=v />, ~children);

test("Rendering wrapped component with prop", () => {
  let rootNode = createRootNode();
  let container = TestReact.createContainer(rootNode);

  let expectedStructure: tree(primitives) =
    TreeNode(
      Root,
      [TreeLeaf(A(7))],
    );

  TestReact.updateContainer(container, <componentWrappingAWithProps v=7 />);

  validateStructure(rootNode, expectedStructure);
});

let componentWithWrappedComponents = (~children, ()) => TestReact.component(() => {
    <aComponent testVal=1>
        <componentWrappingB />
      </aComponent>
}, ~children);

test("Rendering wrapped component with wrappedComponent as child prop", () => {
  let rootNode = createRootNode();
  let container = TestReact.createContainer(rootNode);

  let expectedStructure: tree(primitives) =
    TreeNode(
      Root,
      [TreeNode(A(1), [TreeLeaf(B)])]
    );

  TestReact.updateContainer(container, <componentWithWrappedComponents />);

  validateStructure(rootNode, expectedStructure);
});

let componentThatRendersChildren = (~children, ()) => TestReact.component(() => {
    <aComponent testVal=1>
        ...children
    </aComponent>
}, ~children);

test("Rendering component that renders primitive child", () => {
  let rootNode = createRootNode();
  let container = TestReact.createContainer(rootNode);

  let expectedStructure: tree(primitives) =
    TreeNode(
      Root,
      [TreeNode(A(1), [TreeLeaf(B)])]
    );

  TestReact.updateContainer(container, <componentThatRendersChildren><bComponent /></componentThatRendersChildren>);

  validateStructure(rootNode, expectedStructure);
});

test("Rendering component that renders component child", () => {
  let rootNode = createRootNode();
  let container = TestReact.createContainer(rootNode);

  let expectedStructure: tree(primitives) =
    TreeNode(
      Root,
      [TreeNode(A(1), [TreeLeaf(B)])]
    );

  TestReact.updateContainer(container, <componentThatRendersChildren><componentWrappingB /></componentThatRendersChildren>);

  validateStructure(rootNode, expectedStructure);
});
