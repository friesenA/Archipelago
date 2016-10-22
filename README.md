- [Project Description](#description)
- [Resources](#resources)
- [Git Convention] (#git-convention)


## Description

The goal of the project is to enhance your understanding of the computer graphics principles and your graphics
systems programming skills. You will do this by designing and implementing a project which emphasises certain
aspects of computer graphics. You carry out the project in a team. Your team can choose from one of the two project
topics given by the instructors on the next page. You MUST​ select from one of these projects and not something of
your own, but you are expected to make improvisations of your own within your chosen project.
You must​ submit a project implementation plan/outline in week # 5. This will be included in the final grading of the
project. The outline should be a short, two page report describing what and how your team plans to carry out the
project. It should also include a tentative timeline of at least three milestones. Note that you may be asked to modify
the proposed plan prior to the approval. Only teams whose proposals need modification will be contacted.
At the end of the semester, you must submit a written report of about 5 pages, covering what your objective was, why
it interested you, how you accomplished your objective, what you learned as a result, and the list of references and
resources that you used. You will also have to present/demo your project to the instructors, TAs, and the rest of the
class in the last week of classes.  All team members MUST participate in this final demo. The demo will be scheduled
in the lab. Only demonstrated and running programs will be evaluated.

The project is to be completed by a team of 4­5 students. In exceptional situations, 3 student teams will be permitted.
The project implementation report should clearly state what each member of the group will be working on.
Submission of Project Implementation Report
Submit your project implementation report using Moodle.

#### Submission of Final Project
Please create a zip file containing your C/C++ code, vertex shader, fragment shader, a readme text file (.txt) and
representative screen shots of your project in action. In the readme file, document the input controls e.g.
keyboard/mouse shortcuts, etc.
Submission instructions will be provided on the course website.
COMP371: Computer Graphics, 2016 Fall C. Poullis Team Project

#### PROJECT TOPICS
1. Interior Furniture Layout Design in 3D
The goal of this project is to create an interactive tool using OpenGl for layout of furniture in a room, say living
room, dining room, bedroom, or an outdoor landscape, sports field with players, etc. The tool will allow a user
(designer) to pick objects from a small repository, and then orient and position them on the floor/terrain or the
wall depending on the object type. The designer should also be able to choose colours/textures to the walls,
ceiling and floor/terrain. The room can be viewed from any angle. You are allowed to borrow digital assets
(like the furniture and decorative items, textures, and so on) as needed in order to create the repository. 3D
Warehouse from google is one such source for such 3D models. However ensure that you credit the source
properly. Items when being moved within the room for repositioning should not be allowed to pass through
other items or through walls, floors, ceiling etc. The functionality to interactively select an object from a
repository and to position/orient it at a desired location is mandatory. Absence of this functionality will result in
a reduced score for the project.

2. Walk Through a Procedurally Modelled World
The goal of this project is to create a program using OpenGl for walking through a procedurally created virtual
world, say a forest of trees, plants, bushes etc., or an urban city with roads, buildings, etc. All repeating items,
like the trees in the virtual forest or the buildings in the virtual city, should be procedurally created, using a few
user specified parameters, if necessary. The terrain must be procedurally created. Please look up the web to
get a clear idea of what is procedural modeling. If our project does not clearly demonstrate procedural
modeling, your project will get a reduced score. You are allowed to borrow digital assets for non­repeating
items, like a bridge, for textures, and so on, as needed in order to create the virtual world. However ensure
that you credit the source properly. You should have simple interaction mechanisms to navigate through the
virtual world and view it from different angles. During navigation, the user must not be allowed to pass through
objects.


## Resources

- Video: https://www.youtube.com/watch?v=-d2-PtK4F6Y
- Blog city generation: http://www.shamusyoung.com/twentysidedtale/?p=2940
- Blog landscape generation: http://www.shamusyoung.com/twentysidedtale/?p=11874
- Procedural Terrain Generation (Perlin Noise): https://flafla2.github.io/2014/08/09/perlinnoise.html
- Procedural Object Placement: http://devmag.org.za/2009/04/25/perlin-noise/


## Git convention

- 1. For each new or existing feature, create a new issue
- 2. Open the assigned or newly created issue and look for the button `New ranch`
    _(this will create a new branch using the updated `master` branch. Note that work in progress from other branches are not reflected)_
- 3. Once a feature is completed, update your local branch with the master branch
    _(we want to make sure that your new feature does not break or come into conflict with completed features in the `master` branch)_
- 4. Create a new merge resquest to merge your changes to the `master` branch
- 5. The merge resquest will be reviewed and accepted by people with the profil `master`
=======

