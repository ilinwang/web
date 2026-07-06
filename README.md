# I-Lin Wang — Personal Academic Website

A static, responsive personal homepage rebuilt from the CV data in
`nsc/C302_260210.docx` (content current to 2026). No build step, no
dependencies — just plain HTML + one CSS file.

## Files

```
ilin_homepage/
├─ index.html            Home / About (bio, education, appointments, honors,
│                        prospective-student info)
├─ research.html         Research interests + NSC/NSTC & industry grants
├─ publications.html     Journal (28), conference (142), thesis/books
├─ awards.html           Personal honors + student thesis/project awards
├─ students.html         Advising summary + 80 supervised master's theses
├─ css/style.css         All styling (palette inherited from the original
│                        site: grey page, #646464 bars, daffodil yellow)
├─ images/photo.jpg      Profile photo
├─ images/banner.jpg     Daffodil banner from the original site header
└─ files/student_bg1.pdf 新生背景調查表 (background survey for prospective
                         graduate students)
```

## Publish to GitHub Pages

1. Create a repo. For your main site use the name `<username>.github.io`;
   for a project site any name works (it will live at
   `https://<username>.github.io/<repo>/`).
2. Copy **everything inside this `ilin_homepage/` folder** into the repo root
   (so that `index.html` sits at the top level of the repo).
3. Commit and push.
4. On GitHub: **Settings → Pages → Build and deployment → Source = Deploy from a
   branch**, pick `main` and `/ (root)`, then **Save**.
5. Your site goes live at the URL shown on that page in a minute or two.

All links are relative, so the site works whether it is served from a user
site (`username.github.io`) or a project subpath.

## Updating content

Text lives directly in the `.html` files — edit and re-push. The list content
was generated from the Word document; if you regenerate it later, the year tags
are auto-derived from the first year in each entry.
