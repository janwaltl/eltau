# TODO list

## Minimal set
- [x] Write-only terminal interface.
- [x] Unicode is optional, support ASCII-only.
  - [x] Refactor `Text` into `eltau::ascii`.
  - [x] Support newlines, no wrapping.
  - [x] No styles.
  - [ ] Drawing.
- [ ] Implement a proper eager terminal.
  - [ ] Design the main event loop.
- [ ] Implement a reactive terminal.
  - [ ] Design the main event loop.
- [ ] Implement horizontal, vertical containers.
  - [ ] Test how multiple text elements look like.
- [ ] Support for stack-based styles.
- [ ] Add layout modifiers - centering, flex, border.
- [ ] Add Text button.

## Core features
- Minimal redraw, draw only what changed
- Small footprint if possible, no frame-level allocs. 
- Reasonable UTF-8 support.


## Reactive terminal
Optimized for CPU and serial throughput with minimal redraws.

- [ ] Think through the current `get_preferred_size`, lay out the elements, `draw(actual size)`.
  - [ ] Can it always be done? Is one iteration of pref+draw enough?
  - [ ] Eliminate double calls, draw calculates similar stuff as preferred size.
    - On one hand, try avoid introducing state, on the other, explicit `last_calc_size` might be handy.
- [ ] Think about state propagation and clear trigger for re-draw.
  - [ ] Resolve the propagation of re-draw - when it stops? Interops with `pref_size`?
- [ ] Implement proof of concept reactive terminal.
- [ ] Verify, element size is calculated only if it will be drawn.
  - Either because it is dirty and needs to be redrawn,
  - or because neighbours changed.
  
// Notes:
// Is container aligned or should parent control that?
// Element | hcenter | vcenter | hleft | hright | vup | vdown | hflex | vflex(weight) |  center | flex | scrollable
//-> AlignedElement
//	- Or just set fields of every element <- YEA.


## Functional approach
Focus on eliminating as much statefullness as possible. Simple GUI must be implementable with simple code.

- Dynamic opening/closing of windows, screens, sections, popups.
- On-the-fly elements.
  - Careful about ABA problems with e.g. `Text` having the same `preferred_size` but different content. 
    - Perhaps `DynamicContext` with hash-based cache? Ideally, the context is introduced automatically or through |.
- Callbacks for buttons, effect on reactive terminal.
- At least prioritize composability, monadic behaviour.
- Disentagle GUI state from logic state.
- User always reacts to the last drawn screen - keeping last frame data around is important for input - mouse, keyboard.


