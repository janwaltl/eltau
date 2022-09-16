# TODO list

## Minimal set
- [ ] Fix `Text` drawing.
- [ ] Test `Text` drawing.
- [ ] Implement a proper eager terminal.
- [ ] Implement horizontal, vertical containers.
- [ ] Add Text button.
- [ ] Add layout modifiers - centering, flex, border.

## Reactive terminal
Optimized for CPU and serial throughput with minimal redraws.

- [ ] Think through the current `get_preferred_size`, lay out the elements, `draw(actual size)`.
  - [ ] Can it always be done? Is one iteration of pref+draw enough?
  - [ ] Eliminate double calls, draw calculates similar stuff as preferred size.
    - On one hand, try avoid introducing state, on the other, explicit `last_calc_size` might be handy.
- [ ] Think about state propagation and clear trigger for re-draw.
  - [ ] Resolve the propagation of re-draw - when it stops? Interops with `pref_size`?
- [ ] Implement proof of concept reactive terminal.


## Functional approach
Focus on eliminating as much statefullness as possible. Simple GUI must be implementable with simple code.
- Dynamic opening/closing of windows, screens, sections, popups.
- On-the-fly elements.
  - Careful about ABA problems with e.g. `Text` having the same `preferred_size` but different content. 
    - Perhaps `DynamicContext` with hash-based cache? Ideally, the context is introduced automatically or through |.
- Callbacks for buttons, effect on reactive terminal.
- At least prioritize composability, monadic behaviour.

