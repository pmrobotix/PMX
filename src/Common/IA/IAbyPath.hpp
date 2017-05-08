/*
 * IAbyPath.hpp
 *
 *  Created on: 4 mai 2017
 *      Author: pmx
 */

#ifndef COMMON_IA_IABYPATH_HPP_
#define COMMON_IA_IABYPATH_HPP_

class Robot;

/*
 class SVGTest {
 public:
 virtual void toSVG(Playground * p, FoundPath * found_path, std::string file_name) {
 PathFinder * pf = p->get_path_finder();
 std::vector<Zone*>::iterator zones_it;
 std::vector<Edge*>::iterator edges_it;
 std::vector<Node*>::iterator nodes_it;
 unsigned int i;
 svg::Dimensions dimensions(pf->field_x2, pf->field_y2);

 svg::Layout lay(dimensions, svg::Layout::TopLeft);

 svg::Document doc(file_name, lay);

 doc << svg::elemStart("g") << svg::attribute("transform","translate(200,2200) scale(1,-1)") << svg::emptyElemEnd(false) ;

 // Red image border.
 svg::Polygon border(svg::Fill(svg::Color::White), svg::Stroke(5, svg::Color::Red));
 border << svg::Point(pf->field_x1, pf->field_y1) << svg::Point(dimensions.width, pf->field_y1)
 << svg::Point(dimensions.width, dimensions.height) << svg::Point(pf->field_x1, dimensions.height);
 doc << border;

 // Display all zones
 for (zones_it = pf->zones.begin(); zones_it < pf->zones.end(); zones_it++) {
 Zone* zone = *zones_it;
 if (zone->nodes_count > 0) {
 svg::Polygon zone_poly(svg::Fill(svg::Color::Aqua), svg::Stroke(0, svg::Color::Aqua));
 for (i = 0; i < zone->nodes_count; i++) {
 Node* node = zone->nodes[i];
 zone_poly << svg::Point(node->x, node->y);
 }
 doc << zone_poly;
 }
 }

 // Display all enabled edges
 for (edges_it = pf->edges.begin(); edges_it < pf->edges.end(); edges_it++) {
 Edge* edge = *edges_it;
 if (edge->enabled) {
 svg::Point p1(edge->node1->x, edge->node1->y);
 svg::Point p2(edge->node2->x, edge->node2->y);
 svg::Line line(p1, p2, svg::Stroke(0.5, svg::Color(238,238,238)));
 doc << line;
 }
 }

 // Display found path
 if (found_path != NULL) {
 bool node_found = false;
 svg::Polyline path_polyline(svg::Stroke(5, svg::Color::Green));

 for (nodes_it = found_path->path.begin(); nodes_it < found_path->path.end(); nodes_it++) {
 Node* node = *nodes_it;
 if (!node_found) {
 node_found = true;
 }
 path_polyline << svg::Point(node->x, node->y);
 }
 if (node_found) {
 doc << path_polyline;
 }
 }

 doc << svg::elemEnd("g");
 doc.save();
 }
 virtual ~SVGTest() {
 }
 };

 class PlaygroundSizeTest : public SVGTest {
 protected:
 Playground * p;
 PlaygroundObjectID me = Playground::INVALID;

 PlaygroundSizeTest() {
 p = new Playground(190.0, 230.0, 3000.0, 2100.0, 0.02, 1.0);
 }

 virtual ~PlaygroundSizeTest() {
 delete p;
 }
 */

class IAbyPath
{
public:
	IAbyPath(Robot *robot);
	~IAbyPath()
	{
	}

};

#endif /* COMMON_IA_IABYPATH_HPP_ */
